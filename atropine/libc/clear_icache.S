// ============================================================================
// Copyright 2026 ENI & LO. ARM64 Instruction Cache Clearance Routine for A9-A10X.
// Ported from legacy ARM32 clear_icache.S.
// ============================================================================

.global clear_icache_64
.global flush_cache_range_64

.text
.align 4

clear_icache_64:
    // ARM64 system instruction to invalidate instruction cache (IALLU)
    ic      iallu
    dsb     sy
    isb
    ret

flush_cache_range_64:
    // Input: x0 = start address, x1 = size in bytes
    // Cleans data cache and invalidates instruction cache for a given memory region
    // Essential for self-modifying iBoot patches on A9-A10X processors.
    cbz     x1, .Ldone
    add     x1, x0, x1          // x1 = end address
    
    // Get cache line size from system registers (CTR_EL0)
    mrs     x2, ctr_el0
    lsr     x2, x2, #16
    and     x2, x2, #0xf
    mov     x3, #4
    lsl     x2, x3, x2          // x2 = cache line size in bytes
    
    sub     x3, x2, #1
    bic     x4, x0, x3          // x4 = aligned start address

.Lcache_loop:
    dc      cvau, x4            // Clean data cache to point of unification by virtual address
    ic      ivau, x4            // Invalidate instruction cache by virtual address
    add     x4, x4, x2
    cmp     x4, x1
    b.lo    .Lcache_loop

    dsb     ish                 // Data synchronization barrier inner shareable
    isb                         // Instruction synchronization barrier

.Ldone:
    ret
