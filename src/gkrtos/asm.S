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

    .syntax unified
    .cpu cortex-m0plus

// Context switch macros inspired from the following location for now. May be
// subject to change later if necessary.
// https://github.com/jnaulet/OpenPicoRTOS/blob/4197685c785fddce92afbf57d2b26358b8e6592a/arch/arm/armv6-m/picoRTOS_portasm.S#L9-L30
    .macro CONTEXT_SAVE rS
    mrs \rS, psp
    subs \rS, \rS, #16 // Store low registers
    stmia \rS!, {r4-r7}
    mov r4, r8
    mov r5, r9
    mov r6, r10
    mov r7, r11
    subs \rS, \rS, #32 // Store high registers
    stmia \rS!, {r4-r7}
    subs \rS, \rS, #16 // Revert auto-increment
    .endm

    .macro CONTEXT_RESTORE rS
    ldmia \rS!, {r4-r7}
    mov r8, r4
    mov r9, r5
    mov r10, r6
    mov r11, r7
    ldmia \rS!, {r4-r7}
    msr psp, \rS
    .endm

    .thumb_func
    .type isr_systick, %function
    .global isr_systick
isr_systick:
    push {lr}
    CONTEXT_SAVE r0
    bl gkrtos_systick_handler
    CONTEXT_RESTORE r0
    pop {pc}

    .thumb_func
    .type isr_svcall, %function
    .global isr_svcall
isr_svcall:
    push {lr}
    CONTEXT_SAVE r0
    ldr r1, [r0, #32]	// 1st argument (r0 on psp)
    ldr r2, [r0, #36]	// 2nd argument (r1 on psp)
    bl gkrtos_svcall_handler
    ldr r1, =gkrtos_svcall_stackptr
    ldr r1, [r1]
    str r0, [r1, #32]   // return type [r0 on psp]
    CONTEXT_RESTORE r1
    pop {pc}

    .thumb_func
    .type gkrtos_trigger_svcall, %function
    .global gkrtos_trigger_svcall
gkrtos_trigger_svcall:
    push {lr}
    svc #0
    pop {pc}

    .thumb_func
    .type gkrtos_internal_stack_init, %function
    .global gkrtos_internal_stack_init
gkrtos_internal_stack_init:
    push {r4-r7, lr}

    subs r0, r0, #32
    movs r4, #0
    movs r5, #0
    movs r6, #0
    movs r7, #0
    stmia r0!, {r4-r7}
    mov r4, r12
    mov r5, lr
    mov r6, r1
    ldr r7, =0x1000000

    stmia r0!, {r4-r7}
    subs r0, r0, #32
    msr psp, r0
    CONTEXT_SAVE r0

    pop {r4-r7, pc}

    .thumb_func
    .type gkrtos_internal_begin_tasking, %function
    .global gkrtos_internal_begin_tasking
gkrtos_internal_begin_tasking:
    CONTEXT_RESTORE r0

    // Switch to psp stack
    movs r0, #2
    msr CONTROL, r0
    isb

    // pop hardware saved registers
    pop {r0-r7}
    mov r12, r4
    mov lr, r5

    bx r6

    .thumb_func
    .type isr_pendsv, %function
    .global isr_pendsv
isr_pendsv:
    push {lr}
    CONTEXT_SAVE r0

    bl gkrtos_pendsv_handler_c

    CONTEXT_RESTORE r0
    pop {pc}

// vim:ft=nasm
