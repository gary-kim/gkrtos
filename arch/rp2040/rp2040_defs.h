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

#ifndef GKRTOS_RP2040_DEFS_H
#define GKRTOS_RP2040_DEFS_H
#include "pico/stdlib.h"

#define GKRTOS_SPINLOCK_ID_MINIMUM 16
#define GKRTOS_SPINLOCK_ID_MAXIMUM 31

#define GKRTOS_SPINLOCK_ID_OS1 PICO_SPINLOCK_ID_OS1
#define GKRTOS_SPINLOCK_ID_OS2 PICO_SPINLOCK_ID_OS2

#define GKRTOS_SYSTICK_PRIORITY 1
#define GKRTOS_SVCALL_PRIORITY 1
#define GKRTOS_PENDSV_PRIORITY 0

#endif
