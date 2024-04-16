#include "spinlock.h"

#include "rp2040/rp2040_defs.h"

#include "hardware/sync.h"
#include "pico/stdlib.h"

uint32_t saved_interrupts;
uint32_t os1_interrupts;
uint32_t os2_interrupts;

enum gkrtos_spinlock_status gkrtos_spinlock_get(gkrtos_spinlock_id_t lock_id) {
  if (lock_id > GKRTOS_SPINLOCK_ID_MAXIMUM ||
      lock_id < GKRTOS_SPINLOCK_ID_MINIMUM) {
    return GKRTOS_SPINLOCK_STATUS_UNAVAILABLE;
  }
  saved_interrupts = save_and_disable_interrupts();
  spin_lock_claim(lock_id);
}

enum gkrtos_spinlock_status gkrtos_spinlock_release(
    gkrtos_spinlock_id_t lock_id) {
  if (lock_id > GKRTOS_SPINLOCK_ID_MAXIMUM ||
      lock_id < GKRTOS_SPINLOCK_ID_MINIMUM) {
    return GKRTOS_SPINLOCK_STATUS_UNAVAILABLE;
  }

  spin_lock_unclaim(lock_id);
  restore_interrupts(saved_interrupts);
}

// Gets the OS spinlock 1 of the architecture
// (e.g. SPINLOCK14 on RP2040).
enum gkrtos_spinlock_status _gkrtos_get_os_spinlock_1() {
  os1_interrupts = save_and_disable_interrupts();
  spin_lock_claim(GKRTOS_SPINLOCK_ID_OS1);
}

// Gets the OS spinlock 2 of the architecture
// (e.g. SPINLOCK15 on RP2040).
enum gkrtos_spinlock_status _gkrtos_get_os_spinlock_2() {
  os2_interrupts = save_and_disable_interrupts();
  spin_lock_claim(GKRTOS_SPINLOCK_ID_OS2);
}

// Gets the OS spinlock 1 of the architecture
// (e.g. SPINLOCK14 on RP2040).
enum gkrtos_spinlock_status _gkrtos_release_os_spinlock_1() {
  spin_lock_unclaim(GKRTOS_SPINLOCK_ID_OS2);
  restore_interrupts(os1_interrupts);
}

// Gets the OS spinlock 2 of the architecture
// (e.g. SPINLOCK15 on RP2040).
enum gkrtos_spinlock_status _gkrtos_release_os_spinlock_2() {
  spin_lock_unclaim(GKRTOS_SPINLOCK_ID_OS2);
  restore_interrupts(os2_interrupts);
}
