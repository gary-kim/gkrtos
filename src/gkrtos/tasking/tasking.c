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
  }
  next_task->accounting.ctx_switch_time = current_time;
  next_task->task_status = GKRTOS_TASKING_STATUS_RUNNING;

  current_core->currently_running_pid = next_task->pid;

  return next_task->stackptr;
}

struct gkrtos_tasking_task* gkrtos_get_current_task() {
  uint32_t core_id = gkrtos_get_cpuid();
  return &gkrtos_task_list[gkrtos_tasking_cores[core_id].currently_running_pid];
}

// Requires OS Spinlock
gkrtos_stackptr_t gkrtos_internal_create_new_stack(
    size_t stack_size, gkrtos_tasking_function_t fn_ptr) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  // TODO: Check if going to the end of the stack is really necessary
  gkrtos_stackptr_t stackptr =
      malloc(stack_size) + stack_size -
      1;  // NOLINT(*-misplaced-pointer-arithmetic-in-alloc)
  gkrtos_internal_stack_init(stackptr, fn_ptr);

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return stackptr;
}

// Requires OS Spinlock
enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
}
