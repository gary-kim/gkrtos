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

#ifndef GKRTOS_GKRTOS_H
#define GKRTOS_GKRTOS_H

// Defines for user programs to override pico-sdk
#define sync_internal_yield_until_before(until) gkrtos_syscall_sleep_for(until);
#define GKRTOS_STACK_SETUP(NAME, BLOCKS)         \
  void* GKRTOS_USER_STACK_##NAME[4096 * BLOCKS]; \
  void* NAME = GKRTOS_USER_STACK_##NAME + (4096 * BLOCKS - 1)

// =========================
// == OS Public Functions ==
// =========================

enum gkrtos_result gkrtos_init();

enum gkrtos_result gkrtos_start();

#endif
