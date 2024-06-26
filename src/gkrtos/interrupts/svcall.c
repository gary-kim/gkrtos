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

#include "svcall.h"

#include "gkrtos/asm.h"
#include "gkrtos/hardware/rp2040.h"
#include "gkrtos/syscalls/syscalls.h"
#include "gkrtos/syscalls/syscalls_impl.h"
#include "hardware/exception.h"
#include "rp2040/rp2040_defs.h"

gkrtos_stackptr_t gkrtos_svcall_stackptr;

gkrtos_syscall_return_t gkrtos_svcall_handler(
    gkrtos_stackptr_t stackptr, enum gkrtos_syscall syscall_number,
    void* args) {
  gkrtos_syscall_return_t tr = gkrtos_internal_syscall(
      gkrtos_tasking_get_current_task(), syscall_number, args);
  gkrtos_svcall_stackptr = stackptr;
  return tr;
}

enum gkrtos_result gkrtos_init_svcall_handler() {
  // Just using isr_svcall
  //  exception_set_exclusive_handler(SVCALL_EXCEPTION, gkrtos_isr_svcall);

  // Set system handler priority
  gkrtos_set_register(M0PLUS_SHPR2_OFFSET, M0PLUS_SHPR2_PRI_11_BITS,
                      M0PLUS_SHPR2_PRI_11_LSB, GKRTOS_SVCALL_PRIORITY);
  return GKRTOS_RESULT_SUCCESS;
}
