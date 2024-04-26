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

#include "syscalls_impl.h"

#include "gkrtos/misc/misc.h"
#include "gkrtos/tasking/tasking.h"
#include "pico/types.h"
#include "syscalls.h"

void gkrtos_internal_syscall(struct gkrtos_tasking_task* task,
                             enum gkrtos_syscall syscall, void* args) {
  // Dispatch to correct function
  // TODO: Switch to function pointer array for faster operation
  switch (syscall) {
    case GKRTOS_SYSCALL_SUICIDE:
      gkrtos_internal_syscall_suicide(task);
      break;
    case GKRTOS_SYSCALL_YIELD:
      gkrtos_internal_syscall_yield(task);
      break;
    case GKRTOS_SYSCALL_KILL:
      gkrtos_internal_syscall_kill(task, (gkrtos_pid_t*)args);
      break;
    case GKRTOS_SYSCALL_SLEEP_UNTIL:
      gkrtos_internal_syscall_sleep_until(task, (absolute_time_t*)args);
      break;
    case GKRTOS_SYSCALL_CREATE_TASK:
      gkrtos_internal_syscall_create_task(
          task, (struct gkrtos_syscall_create_task_args*)args);
  }
}

void gkrtos_internal_syscall_suicide(struct gkrtos_tasking_task* task) {}

void gkrtos_internal_syscall_kill(struct gkrtos_tasking_task* task,
                                  gkrtos_pid_t* pid) {}

void gkrtos_internal_syscall_yield(struct gkrtos_tasking_task* task) {}

void gkrtos_internal_syscall_sleep_until(struct gkrtos_tasking_task* task,
                                         absolute_time_t* milliseconds) {}
void gkrtos_internal_syscall_create_task(
    struct gkrtos_tasking_task* calling_task,
    struct gkrtos_syscall_create_task_args* args) {
  struct gkrtos_tasking_task* new_task =
      gkrtos_tasking_task_new(gkrtos_tasking_priority_user(args->priority));
  new_task->function = args->function;
  new_task->stackptr =
      gkrtos_internal_create_new_stack(args->stack_size, args->function);
  gkrtos_tasking_queue_task(new_task);
  // TODO: Finish
}
