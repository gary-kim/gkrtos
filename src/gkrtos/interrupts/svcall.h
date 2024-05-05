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

#ifndef GKRTOS_INTERRUPTS_SVCALL_H
#define GKRTOS_INTERRUPTS_SVCALL_H

#include "gkrtos/hardware/rp2040.h"
#include "gkrtos/misc/misc.h"
#include "gkrtos/syscalls/syscalls.h"

gkrtos_syscall_return_t gkrtos_svcall_handler(
    gkrtos_stackptr_t stackptr, enum gkrtos_syscall syscall_number, void* args);

enum gkrtos_result gkrtos_init_svcall_handler();

#endif
