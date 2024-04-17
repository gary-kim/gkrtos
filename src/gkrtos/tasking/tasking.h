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

#ifndef GKRTOS_TASKING_H
#define GKRTOS_TASKING_H

#include <stdint-gcc.h>

enum gkrtos_result {
  GKRTOS_RESULT_SUCCESS,
  GKRTOS_RESULT_ERROR,
};

enum gkrtos_tasking_priority {
  GKRTOS_TASKING_PRIORITY_REALTIME,
  GKRTOS_TASKING_PRIORITY_KERNEL,
  GKRTOS_TASKING_PRIORITY_PENDSV,
  GKRTOS_TASKING_PRIORITY_USER,
};

struct gkrtos_tasking_task {
  unsigned int pid;

  // priority is the real priority of the task. This should not change
  // throughout the lifetime of the process.
  enum gkrtos_tasking_priority priority;

  // current_priority is the priority of the process at this moment. This may
  // change in scenarios such as priority inversion.
  enum gkrtos_tasking_priority current_priority;
};

// ===========================
// === OS Public Functions ===
// ===========================

struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority);

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority);

#endif
