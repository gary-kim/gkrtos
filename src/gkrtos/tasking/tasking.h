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

#include "gkrtos/config.h"
#include "gkrtos/misc/misc.h"

enum gkrtos_tasking_priority {
  GKRTOS_TASKING_PRIORITY_REALTIME,
  GKRTOS_TASKING_PRIORITY_KERNEL,
  GKRTOS_TASKING_PRIORITY_PENDSV,
  GKRTOS_TASKING_PRIORITY_USER,
};

typedef uint32_t gkrtos_pid_t;
typedef void (*gkrtos_tasking_function_t)();

struct gkrtos_tasking_task {
  gkrtos_pid_t pid;

  // priority is the real priority of the task. This should not change
  // throughout the lifetime of the process.
  enum gkrtos_tasking_priority priority;

  // current_priority is the priority of the process at this moment. This may
  // change in scenarios such as priority inversion.
  enum gkrtos_tasking_priority current_priority;

  // currently_assigned_core contains the cpuid of the core that this task is
  // currently assigned to. This is not guaranteed to be stable throughout the
  // lifetime of the process.
  uint32_t currently_assigned_core;

  // function stores the actual function to run. For most standard tasks, this
  // is only used once during initial setup. This is more useful for specific
  // frequency tasks that will run the task at a specific frequency (which is
  // mostly a helper method).
  gkrtos_tasking_function_t function;

  // run_frequency stores the desired frequency in Hz at which this task should
  // be run. This will usually be set to zero, meaning that the expectation is
  // that the task function will never return.
  uint32_t run_frequency;
};

extern struct gkrtos_tasking_task gkrtos_task_list[GKRTOS_CONFIG_MAX_TASKS];

// ===========================
// === OS Public Functions ===
// ===========================

struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority);

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority);

#endif
