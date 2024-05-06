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

#include "gkrtos/asm.h"
#include "gkrtos/concurrency/private_spinlock.h"
#include "gkrtos/misc/misc.h"
#include "gkrtos/tasking/tasking.h"
#include "hardware/exception.h"
#include "hardware/regs/m0plus.h"
#include "rp2040/rp2040_defs.h"

gkrtos_stackptr_t gkrtos_pendsv_handler_c(gkrtos_stackptr_t stackptr) {
  gkrtos_critical_section_data_structures_enter_blocking();
  // BEGIN CRITICAL REGION

  uint32_t core_id = gkrtos_get_cpuid();
  struct gkrtos_tasking_core* current_core = &gkrtos_tasking_cores[core_id];
  struct gkrtos_tasking_task* current_task =
      &gkrtos_task_list[current_core->currently_running_pid];
  struct gkrtos_tasking_task* next_task =
      &gkrtos_task_list[current_core->queued_task];

  if (current_core->queued_task == current_task->pid) {
    // Looks like a new task was not queued. Just pop back to the former task.
    return stackptr;
  }

  gkrtos_stackptr_t new_sp = gkrtos_internal_context_switch(
      current_core, current_task, next_task, stackptr);

  // END CRITICAL REGION
  gkrtos_critical_section_data_structures_exit();
  return new_sp;
}

enum gkrtos_result gkrtos_init_pendsv_handler() {
  // Just using isr_pendsv
  //  exception_set_exclusive_handler(PENDSV_EXCEPTION,
  //                                  gkrtos_pendsv_context_switch);

  gkrtos_set_register(M0PLUS_SHPR3_OFFSET, M0PLUS_SHPR3_PRI_14_BITS,
                      M0PLUS_SHPR3_PRI_14_LSB, GKRTOS_PENDSV_PRIORITY);

  return GKRTOS_RESULT_SUCCESS;
}
