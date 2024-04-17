#include "tasking.h"

#include "stdint-gcc.h"

struct gkrtos_tasking_task* gkrtos_tasking_task_new(
    enum gkrtos_tasking_priority priority) {
  unsigned static int current_max_pid = 0;
  struct gkrtos_tasking_task* task = malloc(sizeof(struct gkrtos_tasking_task));
  task->pid = current_max_pid++;
  task->priority = priority;
  task->current_priority = task->priority;
  return task;
}

enum gkrtos_tasking_priority gkrtos_tasking_priority_user(uint8_t priority) {
  return GKRTOS_TASKING_PRIORITY_USER + priority;
}
