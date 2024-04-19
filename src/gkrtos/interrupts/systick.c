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

#include "systick.h"

#include "gkrtos/misc/misc.h"
#include "hardware/exception.h"
#include "hardware/irq.h"
#include "hardware/resets.h"
#include "hardware/structs/systick.h"
#include "hardware/watchdog.h"
#include "pico/stdlib.h"

void gkrtos_systick_handler() {
  // SysTick handling
}

enum gkrtos_result init_systick_handler() {
  exception_set_exclusive_handler(SYSTICK_EXCEPTION, gkrtos_isr_systick);
  // Documented here:
  // https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_m0plus
  systick_hw->csr = ((0x1) |       // ENABLE: enable
                     (0x1 << 1) |  // TICKINT: Enable SysTick Exceptions
                     (0x1 << 2)    // CLKSOURCE: Processor Clockj
  );
  // TODO: Determine a more appropriate value for this.
  systick_hw->rvr = 0x00FFFFFF;
  return GKRTOS_RESULT_SUCCESS;
}
