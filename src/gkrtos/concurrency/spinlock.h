#ifndef GKRTOS_CONCURRENCY_SPINLOCK_H
#define GKRTOS_CONCURRENCY_SPINLOCK_H
#include <stdint-gcc.h>

// Identities a specific spinlock
typedef uint8_t gkrtos_spinlock_id_t;

enum gkrtos_spinlock_status {
  GKRTOS_SPINLOCK_STATUS_ACQUIRED,
  GKRTOS_SPINLOCK_STATUS_CANNOT_ACQUIRE,
  GKRTOS_SPINLOCK_STATUS_RELEASED,
  GKRTOS_SPINLOCK_STATUS_UNAVAILABLE
};

// ===========================
// === OS Public Functions ===
// ===========================

int gkrtos_get_spinlock(int lock_id);

#endif
