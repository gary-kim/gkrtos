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

#include "gkrtos/concurrency/private_spinlock.h"
#include "gkrtos/config.h"

struct gkrtos_tasking_task gkrtos_task_list[GKRTOS_CONFIG_MAX_TASKS];

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
  return task;
}

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority) {
  return GKRTOS_TASKING_PRIORITY_USER + priority;
}

// TODO: Implement!!!
gkrtos_stackptr_t gkrtos_internal_context_switch() { return NULL; }

// Requires OS Spinlock
enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
}
