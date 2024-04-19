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
#include "syscalls.h"

void gkrtos_internal_syscall(enum gkrtos_syscall syscall, void* args) {
  // Dispatch to correct function
  // TODO: Switch to function pointer array for faster operation
  switch (syscall) {
    case GKRTOS_SYSCALL_SUICIDE:
      gkrtos_internal_syscall_kill();
      break;
    case GKRTOS_SYSCALL_YIELD:
      gkrtos_internal_syscall_yield(args) : break;
    case GKRTOS_SYSCALL_KILL:
      gkrtos_internal_syscall_kill(args);
      break;
    case GKRTOS_SYSCALL_SLEEP_FOR:
      gkrtos_internal_syscall_sleep_for(args);
      break;
  }
}

void gkrtos_internal_syscall_suicide() {
}

void gkrtos_internal_syscall_kill(gkrtos_pid_t* pid) {
}

void gkrtos_internal_syscall_yield() {
}

void gkrtos_internal_syscall_sleep_for(uint32_t* milliseconds) {
}
