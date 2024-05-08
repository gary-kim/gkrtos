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

#include "syscalls.h"

#include <stddef.h>

#include "gkrtos/asm.h"
#include "gkrtos/misc/misc.h"

bool gkrtos_tasking_started = false;

gkrtos_syscall_return_t gkrtos_syscall(enum gkrtos_syscall syscall,
                                       void* args) {
  if (!gkrtos_tasking_started) {
    return gkrtos_internal_syscall(gkrtos_tasking_get_current_task(), syscall,
                                   args);
  }
  return gkrtos_trigger_svcall(syscall, args);
}

gkrtos_syscall_return_t gkrtos_syscall_die() {
  return gkrtos_syscall(GKRTOS_SYSCALL_DIE, NULL);
}

gkrtos_syscall_return_t gkrtos_syscall_kill(gkrtos_pid_t pid) {
  return gkrtos_syscall(GKRTOS_SYSCALL_KILL, &pid);
}

gkrtos_syscall_return_t gkrtos_syscall_yield() {
  return gkrtos_syscall(GKRTOS_SYSCALL_YIELD, NULL);
}

gkrtos_syscall_return_t gkrtos_syscall_sleep_until(
    absolute_time_t milliseconds) {
  return gkrtos_syscall(GKRTOS_SYSCALL_SLEEP_UNTIL, &milliseconds);
}

gkrtos_pid_t gkrtos_syscall_create_task(
    struct gkrtos_syscall_create_task_args* args) {
  return (gkrtos_pid_t)gkrtos_syscall(GKRTOS_SYSCALL_CREATE_TASK, args);
}
