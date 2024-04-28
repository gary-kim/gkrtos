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

#include "rp2040.h"

#include "gkrtos/config.h"
#include "hardware/clocks.h"
#include "hardware/structs/sio.h"
#include "hardware/sync.h"

uint32_t gkrtos_get_cpuid() { return sio_hw->cpuid; }

uint32_t gkrtos_get_cpu_frequency() { return clock_get_hz(clk_sys); }

uint32_t gkrtos_get_systick_rvr_value() {
  return gkrtos_get_cpu_frequency() / GKRTOS_SYSTICK_FREQUENCY_HZ
}
