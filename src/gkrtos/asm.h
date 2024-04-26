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

#ifndef GKRTOS_ASM_H
#define GKRTOS_ASM_H

#include "gkrtos/misc/misc.h"
#include "gkrtos/syscalls/syscalls.h"
#include "gkrtos/tasking/tasking.h"

// Initialize a new stack for a task
void gkrtos_internal_stack_init(gkrtos_stackptr_t stackptr,
                                gkrtos_tasking_function_t fn_ptr);

// PENDSV interrupt handler
void gkrtos_pendsv_context_switch();

// SVCALL interrupt handler
void gkrtos_isr_svcall();

// Systick interrupt handler
void gkrtos_isr_systick();

// Trigger SVCALL interrupt
void gkrtos_trigger_svcall(enum gkrtos_syscall, void* args);

#endif
