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

#ifndef GKRTOS_TASKING_H
#define GKRTOS_TASKING_H

#include <stdint-gcc.h>

#include "gkrtos/config.h"
#include "gkrtos/hardware/rp2040.h"
#include "gkrtos/misc/misc.h"
#include "rp2040/rp2040_defs.h"

enum gkrtos_tasking_priority {
  GKRTOS_TASKING_PRIORITY_REALTIME,
  GKRTOS_TASKING_PRIORITY_KERNEL,
  GKRTOS_TASKING_PRIORITY_PENDSV,
  GKRTOS_TASKING_PRIORITY_USER,
};

enum gkrtos_tasking_status {
  GKRTOS_TASKING_STATUS_RUNNING,
  GKRTOS_TASKING_STATUS_SLEEPING,
  GKRTOS_TASKING_STATUS_COMPLETE,
  GKRTOS_TASKING_STATUS_NOT_READY,
};

typedef uint32_t gkrtos_pid_t;
typedef void (*gkrtos_tasking_function_t)(gkrtos_pid_t pid);

struct gkrtos_tasking_task {
  gkrtos_pid_t pid;

  // priority is the real priority of the task. This should not change
  // throughout the lifetime of the process.
  enum gkrtos_tasking_priority priority;

  // current_priority is the priority of the process at this moment. This may
  // change in scenarios such as priority inversion.
  enum gkrtos_tasking_priority current_priority;

  // currently_assigned_core contains the cpuid of the core that this task is
  // currently assigned to. This is not guaranteed to be stable throughout the
  // lifetime of the process.
  uint32_t currently_assigned_core;

  // task_status contains the current status of the task.
  enum gkrtos_tasking_status task_status;

  // function stores the actual function to run. For most standard tasks, this
  // is only used once during initial setup. This is more useful for specific
  // frequency tasks that will run the task at a specific frequency (which is
  // mostly a helper method).
  gkrtos_tasking_function_t function;

  // run_frequency stores the desired frequency in Hz at which this task should
  // be run. This will usually be set to zero, meaning that the expectation is
  // that the task function will never return.
  uint32_t run_frequency;

  // stackptr keeps track of the stack pointer whenever a context switch away
  // from this task occurs.
  gkrtos_stackptr_t stackptr;

  // stackbase keeps track of the base of the stack for this process. Mainly
  // used for freeing the stack space later.
  // TODO: Actually use this. Currently unused for simplicity
  gkrtos_stackptr_t stackbase;

  // next_run_time contains the time at which this task must be queued next.
  // This value is set to zero to indicate that there is no timing
  // requirement. If the function must be run by a specific time, this will
  // be set to the time at which it must be run. Then, on the systick before
  // this time is reached, the task will be context switched to.
  // TODO: Actually set this up
  absolute_time_t next_run_time;

  struct {
    absolute_time_t ctx_switch_time;
    uint64_t run_ticks;
  } accounting;
};

struct gkrtos_tasking_core {
  gkrtos_pid_t currently_running_pid;
  gkrtos_pid_t queued_task;
};

extern struct gkrtos_tasking_task gkrtos_task_list[GKRTOS_CONFIG_MAX_TASKS];
extern struct gkrtos_tasking_core gkrtos_tasking_cores[GKRTOS_ARCH_NUM_CORES];
extern struct gkrtos_list* gkrtos_tasking_unscheduled_queue;
extern struct gkrtos_list* gkrtos_tasking_scheduled_queue;

// IMPORTANT: Assumes called in a critical region
gkrtos_stackptr_t gkrtos_internal_context_switch(
    struct gkrtos_tasking_core* current_core,
    struct gkrtos_tasking_task* current_task,
    struct gkrtos_tasking_task* next_task, gkrtos_stackptr_t current_task_sp);

enum gkrtos_result gkrtos_internal_tasking_init();

// ===========================
// === OS Public Functions ===
// ===========================

static inline bool gkrtos_internal_tasking_is_scheduled_task(
    struct gkrtos_tasking_task* task) {
  return !is_nil_time(task->next_run_time);
}

struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority);

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority);

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_internal_tasking_get_next_task();

// Requires OS Spinlock
struct gkrtos_tasking_task* gkrtos_internal_queue_context_switch(
    struct gkrtos_tasking_task* task);

// Requires OS Spinlock
// Returns: Next task to schedule
struct gkrtos_tasking_task* gkrtos_internal_tasking_sleep_until(
    struct gkrtos_tasking_task* task, absolute_time_t milliseconds);

static inline struct gkrtos_tasking_core* gkrtos_tasking_get_current_core() {
  uint32_t core_id = gkrtos_get_cpuid();
  return &gkrtos_tasking_cores[core_id];
}

static inline struct gkrtos_tasking_task* gkrtos_tasking_get_current_task() {
  return &gkrtos_task_list[gkrtos_tasking_get_current_core()
                               ->currently_running_pid];
}

enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task);

enum gkrtos_result gkrtos_tasking_dequeue_task(
    struct gkrtos_tasking_task* task);

// Requires OS Lock
// Returns: whether a reschedule must occur
bool gkrtos_tasking_reschedule_required();

#endif
