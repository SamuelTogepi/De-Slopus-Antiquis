// ============================================================================
// Copyright 2026 ENI & LO. ARM64 Payload Entry Stub for A9-A10X iBoot.
// Ported from legacy 32-bit position-independent GOT-relocating start.S.
// ============================================================================

.global _start
.global _start64

.text
.align 4

_start:
_start64:
    // Save general-purpose registers and link register onto the stack
    stp     x0, x1, [sp, #-16]!
    stp     x2, x3, [sp, #-16]!
    stp     x4, x5, [sp, #-16]!
    stp     x6, x7, [sp, #-16]!
    stp     x29, x30, [sp, #-16!]
    mov     x29, sp

    // In ARM64, Position Independent Code (PIC) uses ADRP/ADD for relative addressing,
    // so manual GOT base relocation loops are unnecessary. We ensure instruction cache
    // is cleared for any relocated or freshly loaded payload sections.
    ic      iallu
    dsb     sy
    isb

    // Extract arguments from stack if passed by iBoot command handler context
    ldp     x0, x1, [sp, #48]
    ldp     x2, x3, [sp, #64]

    // Branch into main payload entry point
    bl      main

    // Store return value from main back into saved x0 slot on stack
    str     x0, [sp, #48]

    // Restore registers and return control back to iBoot caller
    ldp     x29, x30, [sp], #16
    ldp     x6, x7, [sp], #16
    ldp     x4, x5, [sp], #16
    ldp     x2, x3, [sp], #16
    ldp     x0, x1, [sp], #16
    ret
