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

struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority) {
  unsigned static int current_max_pid = 0;
  struct gkrtos_tasking_task* task = malloc(sizeof(struct gkrtos_tasking_task));
  task->pid = current_max_pid++;
  task->priority = priority;
  task->current_priority = task->priority;
  return task;
}

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority) {
  return GKRTOS_TASKING_PRIORITY_USER + priority;
}

enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  gkrtos_get_os_spinlock_data_structures();

  // BEGIN CRITICAL REGION

  // END CRITICAL REGION

  gkrtos_release_os_spinlock_data_structures();
}
