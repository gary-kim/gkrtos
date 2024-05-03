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

#include "runner.h"

#include "gkrtos/syscalls/syscalls.h"
#include "tasking.h"

// task_runner actually runs the function of each task. This allows the RTOS to
// more easily handle things like fixed frequency tasking and not causing issues
// by returning (just call suicide on behalf of the task).
// This task runs in userspace
void gkrtos_internal_task_runner() {
  struct gkrtos_tasking_task *task = gkrtos_tasking_get_current_task();
  task->function();
  gkrtos_syscall_suicide();
}
