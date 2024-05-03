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

#include "gkrtos/asm.h"
#include "gkrtos/hardware/rp2040.h"
#include "gkrtos/misc/misc.h"
#include "hardware/exception.h"
#include "hardware/regs/m0plus.h"
#include "hardware/structs/systick.h"
#include "rp2040/rp2040_defs.h"

gkrtos_stackptr_t gkrtos_systick_handler(gkrtos_stackptr_t stackptr) {
  uint64_t current_time = time_us_64();
  struct gkrtos_tasking_task* current_task = gkrtos_tasking_get_current_task();
  uint64_t elapsed_task_time =
      current_time - current_task->accounting.ctx_switch_time;
  if (elapsed_task_time > GKRTOS_TASKING_TASK_ELAPSED_TIME_US) {
    struct gkrtos_tasking_task* next_task = gkrtos_tasking_get_next_task();
    gkrtos_internal_queue_context_switch(next_task);
  }
  return stackptr;
}

enum gkrtos_result gkrtos_init_systick_handler() {
  exception_set_exclusive_handler(SYSTICK_EXCEPTION, gkrtos_isr_systick);
  // Documented here:
  // https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_m0plus
  systick_hw->csr = ((0x1) |       // ENABLE: enable
                     (0x1 << 1) |  // TICKINT: Enable SysTick Exceptions
                     (0x1 << 2)    // CLKSOURCE: Processor Clock
  );

  // Make sure the value isn't so large that it can't be used
  assert(gkrtos_get_systick_rvr_value() < 0x00FFFFFF);

  systick_hw->rvr = 0x00FFFFFF;

  // Set system handler priority
  gkrtos_set_register(M0PLUS_SHPR3_OFFSET, M0PLUS_SHPR3_PRI_15_BITS,
                      M0PLUS_SHPR3_PRI_15_LSB, GKRTOS_SYSTICK_PRIORITY);

  return GKRTOS_RESULT_SUCCESS;
}
