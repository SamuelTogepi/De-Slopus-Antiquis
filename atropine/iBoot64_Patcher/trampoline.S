// ============================================================================
// Copyright 2026 ENI & LO. ARM64 iBoot Command Trampoline for A9-A10X.
// Ported from legacy 32-bit Thumb/ARM assembly trampoline.S.
// ============================================================================

.global _trampoline_start
.global _trampoline_end

.set GO_SEARCH,             0x006f6700          // \0go\0 string pattern
.set IBEC_LOAD_ADDRESS,     0x80000000          // Payload load address for A9-A10X
.set JUMPTO_ADDRESS,        0xFEEDFACE          // Patched dynamically by patchers64.c

.text
.align 4

_trampoline_start:
    // Save registers and establish stack frame
    stp     x29, x30, [sp, #-32]!
    stp     x19, x20, [sp, #16]
    mov     x29, sp

    // Preserve argument registers passed into the command handler context
    mov     x19, x1                     // Save haystack base (iboot buffer)

    // Find the \0go\0 command string in memory
    ldr     w0, =GO_SEARCH
    mov     x1, x19
    bl      _asm_find
    
    // Calculate offset relative to base, adjust pointer to point to 'go\0'
    sub     x0, x0, x19
    add     x5, x0, #1                  // x5 = relative offset to "go\0"
    
    // Retrieve base address of iBoot image
    mov     x0, x19
    bl      _asm_get_base_address
    add     x0, x0, x5                  // x0 = absolute address of "go\0" string reference
    mov     x1, x19
    bl      _asm_find                   // Find the command struct pointer referencing the string

    // Overwrite the command function handler pointer with our target load address
    ldr     x5, =IBEC_LOAD_ADDRESS
    str     x5, [x0, #8]                // In ARM64, command struct pointers are 64-bit (offset #8)

    // Restore registers and tear down stack frame
    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #32

    // Branch to original jumpto routine (patched dynamically)
    ldr     x16, =JUMPTO_ADDRESS
    br      x16

_asm_get_base_address:
    // Input: x0 = iboot_buf base
    // Extracts base address from Mach-O header / iBoot header structures at offset 0x20
    add     x1, x0, #0x20
    ldr     w2, [x1]
    ldr     x3, =0xFFFFFFFFFFF00000     // 64-bit page mask alignment
    and     x0, x3, x2                  // Clean base address result in x0
    ret

_asm_find:
    // Input: w0 = 32-bit needle pattern, x1 = haystack pointer
    // Scans memory sequentially for the needle
    mov     w2, w0
_find_loop:
    ldr     w3, [x1]
    cmp     w3, w2
    b.eq    _find_ret
    add     x1, x1, #1
    b       _find_loop

_find_ret:
    mov     x0, x1
    ret

_trampoline_end:
