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

// This file contains all the implementations for system calls.

#ifndef GKRTOS_INTERRUPTS_SYSCALLS_IMPL_H
#define GKRTOS_INTERRUPTS_SYSCALLS_IMPL_H

#include <stdint-gcc.h>

#include "gkrtos/misc/misc.h"
#include "gkrtos/syscalls/syscalls.h"
#include "gkrtos/tasking/tasking.h"
#include "pico/types.h"

gkrtos_syscall_return_t gkrtos_internal_syscall(
    struct gkrtos_tasking_task* task, enum gkrtos_syscall syscall, void* args);
gkrtos_syscall_return_t gkrtos_internal_syscall_die(
    struct gkrtos_tasking_task* task);
gkrtos_syscall_return_t gkrtos_internal_syscall_kill(
    const struct gkrtos_tasking_task* task, const gkrtos_pid_t* pid);
gkrtos_syscall_return_t gkrtos_internal_syscall_yield(
    struct gkrtos_tasking_task* task);
gkrtos_syscall_return_t gkrtos_internal_syscall_sleep_until(
    struct gkrtos_tasking_task* task, absolute_time_t* milliseconds);
gkrtos_syscall_return_t gkrtos_internal_syscall_create_task(
    struct gkrtos_tasking_task* calling_task,
    struct gkrtos_syscall_create_task_args* args);
gkrtos_syscall_return_t gkrtos_internal_syscall_task_next_tick(
    struct gkrtos_tasking_task* task, const gkrtos_pid_t* pid);
struct gkrtos_list* gkrtos_list_insert_head(struct gkrtos_list* list,
                                            void* raw_item);

#endif
