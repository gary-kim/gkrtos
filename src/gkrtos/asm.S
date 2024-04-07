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

// vim:ft=nasm

