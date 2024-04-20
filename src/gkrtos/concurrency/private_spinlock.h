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

#include "gkrtos/misc/misc.h"
#include "pico/sync.h"
#include "spinlock.h"

#define gkrtos_get_os_spinlock_atomic gkrtos_get_os_spinlock_1
#define gkrtos_get_os_spinlock_data_structures gkrtos_get_os_spinlock_2

#define gkrtos_release_os_spinlock_atomic gkrtos_release_os_spinlock_1
#define gkrtos_release_os_spinlock_data_structures gkrtos_release_os_spinlock_2

extern critical_section_t critical_section_os1;
extern critical_section_t critical_section_os_data_structures;

enum gkrtos_result gkrtos_critical_section_init();

static inline enum gkrtos_result
gkrtos_critical_section_data_structures_enter_blocking() {
  critical_section_enter_blocking(&critical_section_os_data_structures);
  return GKRTOS_RESULT_SUCCESS;
}

static inline enum gkrtos_result
gkrtos_critical_section_data_structures_exit() {
  critical_section_exit(&critical_section_os_data_structures);
  return GKRTOS_RESULT_SUCCESS;
}

#endif
