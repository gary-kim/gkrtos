#ifndef GKRTOS_GKRTOS_H
#define GKRTOS_GKRTOS_H

// Defines for user programs to override pico-sdk
#define sync_internal_yield_until_before(until) gkrtos_syscall_sleep_for(until);

#endif
