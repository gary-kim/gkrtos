#include "tasking.h"

#include <malloc.h>
#include <stdint-gcc.h>

#include "gkrtos/concurrency/private_spinlock.h"

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

enum gkrtos_result gkrtos_tasking_queue_task(struct gkrtos_tasking_task* task) {
  gkrtos_get_os_spinlock_data_structures();

  // BEGIN CRITICAL REGION

  // END CRITICAL REGION

  gkrtos_release_os_spinlock_data_structures();
}
