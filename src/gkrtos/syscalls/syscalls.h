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

#ifndef GKRTOS_INTERRUPTS_SYSCALLS_H
#define GKRTOS_INTERRUPTS_SYSCALLS_H

#include <stdint-gcc.h>

#include "gkrtos/misc/misc.h"
#include "gkrtos/tasking/tasking.h"
#include "pico/types.h"

// enum gkrtos_sycall is effectively the syscall number table. For readability,
// easier non-C implementation support, and stability, all enum values will be
// numbered in this table.
enum gkrtos_syscall {
  GKRTOS_SYSCALL_KILL = 0,
  GKRTOS_SYSCALL_YIELD = 1,
  GKRTOS_SYSCALL_SLEEP_UNTIL = 2,
  GKRTOS_SYSCALL_SUICIDE = 3,
  GKRTOS_SYSCALL_CREATE_TASK = 4,
};

struct gkrtos_syscall_create_task_args {
  enum gkrtos_tasking_priority priority;
  gkrtos_tasking_function_t function;
  uint32_t run_frequency;
};

void gkrtos_syscall(enum gkrtos_syscall syscall, void* args);
void gkrtos_syscall_suicide();
void gkrtos_syscall_kill(gkrtos_pid_t pid);
void gkrtos_syscall_yield();
void gkrtos_syscall_sleep_until(absolute_time_t milliseconds);
void gkrtos_syscall_create_task(struct gkrtos_syscall_create_task_args* args);

#endif
