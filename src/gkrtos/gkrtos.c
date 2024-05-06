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

#include "gkrtos/asm.h"
#include "gkrtos/concurrency/private_spinlock.h"
#include "gkrtos/interrupts/pendsv.h"
#include "gkrtos/interrupts/svcall.h"
#include "gkrtos/interrupts/systick.h"
#include "gkrtos/tasking/runner.h"
#include "hardware/exception.h"
#include "pico/stdlib.h"

GKRTOS_STACK_SETUP(spin_task_stack, 1);

enum gkrtos_result gkrtos_init() {
  if (!(gkrtos_critical_section_init() == GKRTOS_RESULT_SUCCESS &&
        gkrtos_init_pendsv_handler() == GKRTOS_RESULT_SUCCESS &&
        gkrtos_init_svcall_handler() == GKRTOS_RESULT_SUCCESS &&
        gkrtos_internal_tasking_init() == GKRTOS_RESULT_SUCCESS))
    return GKRTOS_RESULT_ERROR;
  struct gkrtos_syscall_create_task_args spin_task_args = {
      .priority = GKRTOS_TASKING_PRIORITY_USER,
      .function = gkrtos_internal_spin_task,
      .stack_base = spin_task_stack,
  };

  gkrtos_syscall_create_task(&spin_task_args);
  return GKRTOS_RESULT_SUCCESS;
}

enum gkrtos_result gkrtos_start() {
  if (gkrtos_init_systick_handler() == GKRTOS_RESULT_SUCCESS) {
    gkrtos_tasking_started = true;

    // Force the first task to run
    gkrtos_tasking_cores[0].queued_task = 1;
    gkrtos_tasking_cores[0].currently_running_pid = 1;
    gkrtos_task_list[1].task_status = 1;
    gkrtos_task_list[1].accounting.ctx_switch_time = get_absolute_time();

    // The next line should never return at this point
    gkrtos_internal_begin_tasking(gkrtos_task_list[1].stackptr);
  }
  return GKRTOS_RESULT_ERROR;
}
