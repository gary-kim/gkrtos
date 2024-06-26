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

#ifndef GKRTOS_HARDWARE_RP2040_H
#define GKRTOS_HARDWARE_RP2040_H

#include <stdint-gcc.h>

#include "gkrtos/config.h"
#include "pico/types.h"

typedef void* gkrtos_stackptr_t;

// =========================
// == OS Public Functions ==
// =========================

uint32_t gkrtos_get_cpuid();

uint32_t gkrtos_get_cpu_frequency();

uint32_t gkrtos_get_systick_rvr_value();

static inline absolute_time_t gkrtos_get_systick_period() {
  return from_us_since_boot(1000000 / GKRTOS_SYSTICK_FREQUENCY_HZ);
}

#endif
