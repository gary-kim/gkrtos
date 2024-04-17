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

#ifndef GKRTOS_CONCURRENCY_SPINLOCK_H
#define GKRTOS_CONCURRENCY_SPINLOCK_H
#include <stdint-gcc.h>

// Identities a specific spinlock
typedef uint8_t gkrtos_spinlock_id_t;

enum gkrtos_spinlock_status {
  GKRTOS_SPINLOCK_STATUS_ACQUIRED,
  GKRTOS_SPINLOCK_STATUS_CANNOT_ACQUIRE,
  GKRTOS_SPINLOCK_STATUS_RELEASED,
  GKRTOS_SPINLOCK_STATUS_UNAVAILABLE
};

// ===========================
// === OS Public Functions ===
// ===========================

int gkrtos_get_spinlock(int lock_id);

#endif
