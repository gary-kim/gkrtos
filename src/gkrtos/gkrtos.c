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

#include "gkrtos.h"

#include <stdio.h>

#include "gkrtos/concurrency/private_spinlock.h"
#include "gkrtos/interrupts/pendsv.h"
#include "gkrtos/interrupts/svcall.h"
#include "gkrtos/interrupts/systick.h"
#include "pico/stdlib.h"

enum gkrtos_result gkrtos_init() {
  if (gkrtos_critical_section_init() == GKRTOS_RESULT_SUCCESS &&
      gkrtos_init_pendsv_handler() == GKRTOS_RESULT_SUCCESS &&
      gkrtos_init_svcall_handler() == GKRTOS_RESULT_SUCCESS &&
      gkrtos_internal_tasking_init() == GKRTOS_RESULT_SUCCESS)
    return GKRTOS_RESULT_SUCCESS;
  return GKRTOS_RESULT_ERROR;
}

enum gkrtos_result gkrtos_start() {
  if (gkrtos_init_systick_handler() == GKRTOS_RESULT_SUCCESS) {
    // TODO: Context switch into the RTOS
  }
  return GKRTOS_RESULT_ERROR;
}

int main() {
  setup_default_uart();
  printf("Hello, world!\n");
  return 0;
}
