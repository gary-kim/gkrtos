#ifndef GKRTOS_RP2040_DEFS_H
#define GKRTOS_RP2040_DEFS_H
#include "pico/stdlib.h"

#define GKRTOS_SPINLOCK_ID_MINIMUM 16
#define GKRTOS_SPINLOCK_ID_MAXIMUM 31

#define GKRTOS_SPINLOCK_ID_OS1 PICO_SPINLOCK_ID_OS1
#define GKRTOS_SPINLOCK_ID_OS2 PICO_SPINLOCK_ID_OS2

#endif
