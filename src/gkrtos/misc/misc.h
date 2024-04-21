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

#ifndef GKRTOS_MISC_MISC_H
#define GKRTOS_MISC_MISC_H

#include "hardware/address_mapped.h"
#include "hardware/regs/m0plus.h"

enum gkrtos_result {
  GKRTOS_RESULT_SUCCESS,
  GKRTOS_RESULT_ERROR,
};

static void inline gkrtos_set_register(io_rw_32 offset, io_rw_32 bits,
                                       io_rw_32 starting_bits_offset,
                                       io_rw_32 value) {
  io_rw_32* reg = (io_rw_32*)(PPB_BASE + offset);
  io_rw_32 reg_val = *reg;
  reg_val = reg_val & ~bits;
  reg_val = reg_val | (value << starting_bits_offset);
  *reg = reg_val;
}

#endif