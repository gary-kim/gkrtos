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

#include "spinlock.h"

#include "gkrtos/misc/misc.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"
#include "pico/sync.h"
#include "rp2040/rp2040_defs.h"

critical_section_t critical_section_os1;
critical_section_t critical_section_os_data_structures;

enum gkrtos_result gkrtos_critical_section_init() {
  critical_section_init_with_lock_num(&critical_section_os1,
                                      GKRTOS_SPINLOCK_ID_OS1);
  critical_section_init_with_lock_num(&critical_section_os_data_structures,
                                      GKRTOS_SPINLOCK_ID_OS2);
  critical_section_exit(&critical_section_os1);
  critical_section_exit(&critical_section_os_data_structures);
  return GKRTOS_RESULT_SUCCESS;
}
