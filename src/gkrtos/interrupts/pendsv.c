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

#include "pendsv.h"

#include "gkrtos/misc/misc.h"
#include "hardware/exception.h"
#include "hardware/regs/m0plus.h"
#include "rp2040/rp2040_defs.h"

gkrtos_stackptr_t gkrtos_pendsv_handler_c(gkrtos_stackptr_t stackptr) {
  uint32_t core_id = gkrtos_get_cpuid();
}

enum gkrtos_result init_pendsv_handler() {
  exception_set_exclusive_handler(PENDSV_EXCEPTION,
                                  gkrtos_pendsv_context_switch);

  gkrtos_set_register(M0PLUS_SHPR3_OFFSET, M0PLUS_SHPR3_PRI_14_BITS,
                      M0PLUS_SHPR3_PRI_14_LSB, GKRTOS_PENDSV_PRIORITY);

  return GKRTOS_RESULT_SUCCESS;
}