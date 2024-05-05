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
#include "gkrtos/structures/list.h"
#include "hardware/exception.h"

struct gkrtos_tasking_task gkrtos_task_list[GKRTOS_CONFIG_MAX_TASKS];
struct gkrtos_tasking_core gkrtos_tasking_core[GKRTOS_ARCH_NUM_CORES];
struct gkrtos_list* gkrtos_tasking_queue;

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority) {
  static gkrtos_pid_t current_max_pid = 0;

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
        (current_time - current_task->accounting.ctx_switch_time) / 1000llu;
    current_task->task_status = GKRTOS_TASKING_STATUS_SLEEPING;
  }
  next_task->accounting.ctx_switch_time = current_time;
  next_task->task_status = GKRTOS_TASKING_STATUS_RUNNING;

  current_core->currently_running_pid = next_task->pid;

  return next_task->stackptr;
}

// Requires OS Spinlock
// Returns: Next task to schedule
struct gkrtos_tasking_task* gkrtos_internal_tasking_sleep_until(
    struct gkrtos_tasking_task* task, absolute_time_t milliseconds) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  task->next_run_time = milliseconds;
  gkrtos_list_rotate(gkrtos_tasking_queue);
  struct gkrtos_tasking_task* new_task =
      gkrtos_list_get_head(gkrtos_tasking_queue);

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return new_task;
}

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_internal_tasking_get_next_task() {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  gkrtos_list_rotate(gkrtos_tasking_queue);
  struct gkrtos_tasking_task* new_task =
      gkrtos_list_get_head(gkrtos_tasking_queue);

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
  irq_set_pending(PENDSV_EXCEPTION);

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return task;
}

// Requires OS Spinlock
enum gkrtos_result gkrtos_internal_tasking_init() {
  gkrtos_tasking_queue = gkrtos_list_new();
  return GKRTOS_RESULT_SUCCESS;
}

// Requires OS Spinlock
enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  // Start with a simple round-robin implementation that ignores user
  // properties. In the long run, this should be capable of using user
  // priorities to fairly time split.
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  gkrtos_list_prepend(gkrtos_tasking_queue, task);

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return GKRTOS_RESULT_SUCCESS;
}

enum gkrtos_result gkrtos_tasking_dequeue_task(
    struct gkrtos_tasking_task* task) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL SECTION

  gkrtos_list_remove(gkrtos_tasking_queue, gkrtos_list_get_item_with_data(
                                               gkrtos_tasking_queue, task));

  // END CRITICAL SECTION
  gkrtos_critical_section_data_structures_exit();
  return GKRTOS_RESULT_SUCCESS;
}
