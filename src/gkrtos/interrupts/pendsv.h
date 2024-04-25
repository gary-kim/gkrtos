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

#ifndef GKRTOS_INTERRUPTS_PENDSV_H
#define GKRTOS_INTERRUPTS_PENDSV_H

#include "gkrtos/misc/misc.h"
#include "gkrtos/tasking/tasking.h"

gkrtos_stackptr_t gkrtos_pendsv_handler_c(gkrtos_stackptr_t stackptr);
void gkrtos_pendsv_context_switch();
#endif
