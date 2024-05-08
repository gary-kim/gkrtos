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

#include "gkrtos/asm.h"
#include "gkrtos/misc/misc.h"
#include "gkrtos/tasking/runner.h"
#include "gkrtos/tasking/tasking.h"
#include "pico/types.h"
#include "syscalls.h"

gkrtos_syscall_return_t gkrtos_internal_syscall(
    struct gkrtos_tasking_task* task, enum gkrtos_syscall syscall, void* args) {
  // Dispatch to correct function
  // TODO: Switch to function pointer array for faster operation
  // or verify that it compiles to the same thing
  switch (syscall) {
    case GKRTOS_SYSCALL_DIE:
      return gkrtos_internal_syscall_die(task);
      break;
    case GKRTOS_SYSCALL_YIELD:
      return gkrtos_internal_syscall_yield(task);
      break;
    case GKRTOS_SYSCALL_KILL:
      return gkrtos_internal_syscall_kill(task, (gkrtos_pid_t*)args);
      break;
    case GKRTOS_SYSCALL_SLEEP_UNTIL:
      return gkrtos_internal_syscall_sleep_until(task, (absolute_time_t*)args);
      break;
    case GKRTOS_SYSCALL_CREATE_TASK:
      return gkrtos_internal_syscall_create_task(
          task, (struct gkrtos_syscall_create_task_args*)args);
  }
  return -GKRTOS_SYSCALL_ERRNO_NO_SUCH_SYSCALL;
}

gkrtos_syscall_return_t gkrtos_internal_syscall_die(
    struct gkrtos_tasking_task* task) {
  gkrtos_internal_syscall_kill(task, &task->pid);
  return GKRTOS_SYSCALL_ERRNO_SUCCESS;
}

gkrtos_syscall_return_t gkrtos_internal_syscall_kill(
    const struct gkrtos_tasking_task* task, const gkrtos_pid_t* pid) {
  struct gkrtos_tasking_task* dying_task = &gkrtos_task_list[*pid];
  dying_task->task_status = GKRTOS_TASKING_STATUS_COMPLETE;

  // Queue up the next task to start (since we are killing the current task)
  struct gkrtos_tasking_task* next_task =
      gkrtos_internal_tasking_get_next_task();

  // Specifically delay actually dequeueing to make sure that the
  // gkrtos_internal_tasking_get_next_task gets the correct next task.
  gkrtos_tasking_dequeue_task(dying_task);

  gkrtos_internal_queue_context_switch(next_task);
  return GKRTOS_SYSCALL_ERRNO_SUCCESS;
}

gkrtos_syscall_return_t gkrtos_internal_syscall_yield(
    struct gkrtos_tasking_task* task) {
  struct gkrtos_tasking_task* next_task =
      gkrtos_internal_tasking_get_next_task();
  gkrtos_internal_queue_context_switch(next_task);
  return GKRTOS_SYSCALL_ERRNO_SUCCESS;
}

gkrtos_syscall_return_t gkrtos_internal_syscall_sleep_until(
    struct gkrtos_tasking_task* task, absolute_time_t* milliseconds) {
  struct gkrtos_tasking_task* next_task =
      gkrtos_internal_tasking_sleep_until(task, *milliseconds);
  gkrtos_internal_queue_context_switch(next_task);
  return GKRTOS_SYSCALL_ERRNO_SUCCESS;
}

gkrtos_syscall_return_t gkrtos_internal_syscall_create_task(
    struct gkrtos_tasking_task* calling_task,
    struct gkrtos_syscall_create_task_args* args) {
  struct gkrtos_tasking_task* new_task =
      gkrtos_tasking_task_new(gkrtos_tasking_priority_user(args->priority));
  new_task->function = args->function;
  new_task->stackptr =
      gkrtos_internal_stack_init(args->stack_base, gkrtos_internal_task_runner);
  new_task->stackbase = args->stack_base;
  new_task->task_status = GKRTOS_TASKING_STATUS_SLEEPING;
  gkrtos_tasking_queue_task(new_task);
  return new_task->pid;
}
