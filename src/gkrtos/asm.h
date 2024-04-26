#ifndef GKRTOS_ASM_H
#define GKRTOS_ASM_H

#include "gkrtos/tasking/tasking.h"

// Initialize a new stack for a task
void gkrtos_internal_stack_init(gkrtos_stackptr_t stackptr,
                                gkrtos_tasking_function_t fn_ptr);

// PENDSV interrupt handler
void gkrtos_pendsv_context_switch();

// SVCALL interrupt handler
void gkrtos_isr_svcall();

// Systick interrupt handler
void gkrtos_isr_systick();

// Trigger SVCALL interrupt
void gkrtos_trigger_svcall(enum gkrtos_syscall, void* args);

#endif
