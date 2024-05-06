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

#include "gkrtos/gkrtos.h"
#include "gkrtos/syscalls/syscalls.h"
#include "gkrtos/tasking/tasking.h"
#include "pico/stdlib.h"

GKRTOS_STACK_SETUP(blink_stack, 2);
void blink() {
  while (true) {
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    gkrtos_syscall_sleep_until(delayed_by_us(get_absolute_time(), 100000));
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    gkrtos_syscall_sleep_until(delayed_by_us(get_absolute_time(), 100000));
  }
}

int main() {
  struct gkrtos_syscall_create_task_args task1_args = {
      .priority = GKRTOS_TASKING_PRIORITY_USER,
      .function = blink,
      .stack_base = blink_stack};
  gkrtos_syscall_create_task(&task1_args);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_put(PICO_DEFAULT_LED_PIN, 1);
  gkrtos_init();
  gkrtos_start();
}
