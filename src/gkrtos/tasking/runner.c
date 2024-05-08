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
// by returning (just call die on behalf of the task).
// This task runs in userspace
void gkrtos_internal_task_runner() {
  struct gkrtos_tasking_task *task = gkrtos_tasking_get_current_task();
  if (task->run_frequency == 0) {
    task->function(task->pid);
    gkrtos_syscall_die();
  } else {
    absolute_time_t next_run = get_absolute_time();
    while (true) {
      next_run = delayed_by_us(next_run, 1000000 / task->run_frequency);
      task->function(task->pid);
      if (absolute_time_diff_us(get_absolute_time(), next_run) >= 0) {
        gkrtos_syscall_sleep_until(next_run);
      }
    }
  }
}

// gkrtos_internal_spin_task is just an empty task that will immediately yield.
// Used as a sleep task, essentially (a very inefficient one but still).
void gkrtos_internal_spin_task() {
  while (true) {
    gkrtos_syscall_yield();
  }
}
