// Copyright (C) 2024 Gary Kim <gary@garykim.dev>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "tasking.h"

#include <malloc.h>
#include <stdint-gcc.h>

#include "gkrtos/asm.h"
#include "gkrtos/concurrency/private_spinlock.h"
#include "gkrtos/config.h"
#include "gkrtos/interrupts/pendsv.h"
#include "gkrtos/structures/list.h"
#include "hardware/exception.h"

struct gkrtos_tasking_task gkrtos_task_list[GKRTOS_CONFIG_MAX_TASKS];
struct gkrtos_tasking_core gkrtos_tasking_cores[GKRTOS_ARCH_NUM_CORES];
struct gkrtos_list* gkrtos_tasking_scheduled_queue;
struct gkrtos_list* gkrtos_tasking_unscheduled_queue;

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority) {
  static gkrtos_pid_t current_max_pid = 1;

  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  gkrtos_pid_t process_pid = current_max_pid++;

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();

  struct gkrtos_tasking_task* task = &gkrtos_task_list[process_pid];
  task->pid = process_pid;
  task->priority = priority;
  task->current_priority = task->priority;
  task->currently_assigned_core = 0;
  task->function = NULL;
  task->run_frequency = 0;
  task->accounting.run_ticks = 0;
  task->stackptr = NULL;
  task->stackbase = NULL;
  task->task_status = GKRTOS_TASKING_STATUS_NOT_READY;
  return task;
}

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority) {
  return GKRTOS_TASKING_PRIORITY_USER + priority;
}

// IMPORTANT: Assumes called in a critical region
gkrtos_stackptr_t gkrtos_internal_context_switch(
    struct gkrtos_tasking_core* current_core,
    struct gkrtos_tasking_task* current_task,
    struct gkrtos_tasking_task* next_task, gkrtos_stackptr_t current_task_sp) {
  current_task->stackptr = current_task_sp;
  current_core->currently_running_pid = current_task->pid;

  uint64_t current_time = time_us_64();

  // Do task accounting
  if (current_task->task_status == GKRTOS_TASKING_STATUS_RUNNING) {
    current_task->accounting.run_ticks +=
        (current_time -
         to_us_since_boot(current_task->accounting.ctx_switch_time)) /
        1000llu;
    current_task->task_status = GKRTOS_TASKING_STATUS_SLEEPING;
  }
  next_task->accounting.ctx_switch_time = get_absolute_time();
  next_task->task_status = GKRTOS_TASKING_STATUS_RUNNING;

  current_core->currently_running_pid = next_task->pid;

  return next_task->stackptr;
}

// IMPORTANT: Assumes called in critical region
bool queue_sort(void* arg1, void* arg2, void* arg3) {
  // Assumes that a list is either all scheduled tasks or unscheduled tasks.
  // Cannot handle mixed lists.
  struct gkrtos_tasking_task* inserting = (struct gkrtos_tasking_task*)arg1;
  struct gkrtos_tasking_task* previous = (struct gkrtos_tasking_task*)arg2;
  struct gkrtos_tasking_task* next = (struct gkrtos_tasking_task*)arg3;

  if (!gkrtos_internal_tasking_is_scheduled_task(inserting)) {
    return true;
  }

  return absolute_time_diff_us(inserting->next_run_time, next->next_run_time) >
             0 &&
         absolute_time_diff_us(previous->next_run_time, next->next_run_time) >=
             0;
}

// IMPORTANT: Assumes called in a critical region
static inline bool gkrtos_internal_tasking_rescheduling_required_locked() {
  struct gkrtos_tasking_task* current_task = gkrtos_tasking_get_current_task();
  uint64_t elapsed_task_time = absolute_time_diff_us(
      current_task->accounting.ctx_switch_time, get_absolute_time());
  if (elapsed_task_time > GKRTOS_TASKING_TASK_ELAPSED_TIME_US) {
    // This task has run for long enough
    return true;
  }
  absolute_time_t next_systick =
      make_timeout_time_us(to_us_since_boot(gkrtos_get_systick_period()));
  // Is the next scheduled task expecting to be rescheduled within the next
  // systick period?
  if (!gkrtos_list_is_empty(gkrtos_tasking_scheduled_queue) &&
      absolute_time_diff_us(((struct gkrtos_tasking_task*)gkrtos_list_get_head(
                                 gkrtos_tasking_scheduled_queue))
                                ->next_run_time,
                            next_systick) > 0) {
    // A scheduled task expects to be on the core soon.
    return true;
  }
  return false;
}

// IMPORTANT: Assumes called in critical region
static inline struct gkrtos_tasking_task*
gkrtos_internal_tasking_get_next_task_locked() {
  // Is the next scheduled task expecting to be rescheduled within the next
  // systick period?
  absolute_time_t next_systick =
      make_timeout_time_us(to_us_since_boot(gkrtos_get_systick_period()));
  if (gkrtos_list_is_empty(gkrtos_tasking_scheduled_queue) ||
      absolute_time_diff_us(((struct gkrtos_tasking_task*)gkrtos_list_get_head(
                                 gkrtos_tasking_scheduled_queue))
                                ->next_run_time,
                            next_systick) <= 0) {
    // We have at least another systick until a scheduled task must be run
    gkrtos_list_rotate(gkrtos_tasking_unscheduled_queue);
    return gkrtos_list_get_head(gkrtos_tasking_unscheduled_queue);
  }

  // We have to schedule the task on gkrtos_tasking_scheduled_queue now!
  struct gkrtos_tasking_task* new_task =
      gkrtos_list_get_head(gkrtos_tasking_scheduled_queue);

  // Remove from scheduled_queue list then add to unscheduled_queue at the end
  gkrtos_list_remove(
      gkrtos_tasking_scheduled_queue,
      gkrtos_list_get_item_with_data(gkrtos_tasking_scheduled_queue, new_task));
  new_task->next_run_time = nil_time;
  gkrtos_list_append(gkrtos_tasking_unscheduled_queue, new_task);
  return new_task;
}

// IMPORTANT: Assumes called in critical region
static inline void gkrtos_internal_tasking_queue_task_locked(
    struct gkrtos_tasking_task* task) {
  if (gkrtos_internal_tasking_is_scheduled_task(task)) {
    gkrtos_list_insert_sorted(gkrtos_tasking_scheduled_queue, task, queue_sort);
  } else {
    gkrtos_list_prepend(gkrtos_tasking_unscheduled_queue, task);
  }
}

static inline void gkrtos_internal_tasking_dequeue_task_locked(
    struct gkrtos_tasking_task* task) {
  if (gkrtos_internal_tasking_is_scheduled_task(task)) {
    gkrtos_list_remove(
        gkrtos_tasking_scheduled_queue,
        gkrtos_list_get_item_with_data(gkrtos_tasking_scheduled_queue, task));
  } else {
    gkrtos_list_remove(
        gkrtos_tasking_unscheduled_queue,
        gkrtos_list_get_item_with_data(gkrtos_tasking_unscheduled_queue, task));
  }
}

// Requires OS Spinlock
// Returns: Next task to schedule
struct gkrtos_tasking_task* gkrtos_internal_tasking_sleep_until(
    struct gkrtos_tasking_task* task, absolute_time_t milliseconds) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  // Remove the task from the unscheduled queue then add it to the scheduled
  // queue
  gkrtos_internal_tasking_dequeue_task_locked(task);
  task->next_run_time = milliseconds;
  gkrtos_internal_tasking_queue_task_locked(task);

  // Find next task to run
  struct gkrtos_tasking_task* new_task =
      gkrtos_internal_tasking_get_next_task_locked();

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return new_task;
}

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_internal_tasking_get_next_task() {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  // Find next task to run
  struct gkrtos_tasking_task* new_task =
      gkrtos_internal_tasking_get_next_task_locked();

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return new_task;
}

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_internal_queue_context_switch(
    struct gkrtos_tasking_task* task) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  struct gkrtos_tasking_core* current_core = gkrtos_tasking_get_current_core();
  current_core->queued_task = task->pid;
  if (current_core->currently_running_pid != task->pid) {
    gkrtos_trigger_pendsv();
  }

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return task;
}

enum gkrtos_result gkrtos_internal_tasking_init() {
  gkrtos_tasking_scheduled_queue = gkrtos_list_new();
  gkrtos_tasking_unscheduled_queue = gkrtos_list_new();
  for (int i = 0; i < GKRTOS_ARCH_NUM_CORES; i++) {
    gkrtos_tasking_cores[i].currently_running_pid = 0;
    gkrtos_tasking_cores[i].queued_task = 0;
  }
  return GKRTOS_RESULT_SUCCESS;
}

// Requires OS Spinlock
enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  // Start with a simple round-robin implementation that ignores user
  // properties. In the long run, this should be capable of using user
  // priorities to fairly time split.
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  gkrtos_internal_tasking_queue_task_locked(task);

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return GKRTOS_RESULT_SUCCESS;
}

enum gkrtos_result gkrtos_tasking_dequeue_task(
    struct gkrtos_tasking_task* task) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  gkrtos_internal_tasking_dequeue_task_locked(task);

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return GKRTOS_RESULT_SUCCESS;
}
bool gkrtos_tasking_reschedule_required() {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  bool required = gkrtos_internal_tasking_rescheduling_required_locked();

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return required;
}
