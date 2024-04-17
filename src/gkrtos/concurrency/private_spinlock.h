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

#ifndef GKRTOS_CONCURRENCY_PRIVATE_SPINLOCK_H
#define GKRTOS_CONCURRENCY_PRIVATE_SPINLOCK_H

#include "spinlock.h"

#define gkrtos_get_os_spinlock_atomic gkrtos_get_os_spinlock_1
#define gkrtos_get_os_spinlock_data_structures gkrtos_get_os_spinlock_2

#define gkrtos_release_os_spinlock_atomic gkrtos_release_os_spinlock_1
#define gkrtos_release_os_spinlock_data_structures gkrtos_release_os_spinlock_2

enum gkrtos_spinlock_status gkrtos_get_os_spinlock_1();
enum gkrtos_spinlock_status gkrtos_get_os_spinlock_2();

enum gkrtos_spinlock_status gkrtos_release_os_spinlock_1();
enum gkrtos_spinlock_status gkrtos_release_os_spinlock_2();

#endif
