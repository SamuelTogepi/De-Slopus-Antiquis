/*
 * Copyright 2026 ENI & LO. Specialized ARM64 iBoot Patcher for A9-A10X.
 * Adapted and ported from legacy 32-bit iBoot patching architectures.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <include/finders64.h>
#include <include/functions64.h>
#include <include/iBoot64Patcher.h>

void* find_cmd_handler_64(void* iboot_buf, size_t iboot_len, const char* cmd_name, size_t cmd_len) {
    /* In ARM64 iBoot, command strings are referenced via literal pools or pointer tables. 
     * We locate the string reference in the binary buffer first. */
    uintptr_t command_str_ref = (uintptr_t)memmem(iboot_buf, iboot_len, cmd_name, cmd_len);
    if (!command_str_ref) {
        return NULL;
    }

    /* Calculate offset relative to the base buffer */
    uint32_t* buf_32 = (uint32_t*)iboot_buf;
    size_t count = iboot_len / sizeof(uint32_t);
    
    /* Search for pointer references pointing to this command string address */
    uintptr_t target_addr = command_str_ref;
    for (size_t i = 0; i < count; i++) {
        if (buf_32[i] == (uint32_t)(target_addr & 0xFFFFFFFF)) {
            // Found potential reference slot in the command struct table
            return (void*)&buf_32[i];
        }
    }
    return NULL;
}

void* find_jumpto_64(struct iboot_img* iboot_in) {
    /* In A9-A10X iBSS/LLB, execution transfer to kernel/iBoot stage uses a standardized 
     * register setup convention. We search for ARM64 branch structures or specific 
     * immediate loader patterns for execution handoff. */
    
    uint8_t* buf = (uint8_t*)iboot_in->buf;
    size_t len = iboot_in->len;

    // Search for ARM64 function prologues and branch-with-link patterns matching boot execution handoff
    void* candidate = arm64_find_jumpto_pattern(buf, len);
    while (candidate) {
        if (arm64_validate_jumpto_context(candidate)) {
            return candidate;
        }
        candidate = arm64_find_jumpto_pattern(candidate + 4, len - (size_t)(candidate - (void*)buf));
    }
    
    return NULL;
}

void* find_bl_verify_shsh_64(struct iboot_img* iboot_in) {
    int os_vers = get_os_version(iboot_in);

    /* iOS 10 through iOS 14+ on A9-A10X uses updated trust evaluation routines. 
     * Route based on major OS version constraints inside the bootloader image. */
    if (os_vers >= 10 && os_vers <= 14) {
        return find_bl_verify_shsh_a9_a10(iboot_in);
    }

    return find_bl_verify_shsh_generic_64(iboot_in);
}

void* find_bl_verify_shsh_a9_a10(struct iboot_img* iboot_in) {
    /* Find the literal reference for 'CERT' or trust evaluation strings in ARM64 constants */
    void* ldr_insn = find_next_ARM64_literal_reference(iboot_in->buf, iboot_in->len, "CERT", 4);
    if (!ldr_insn) {
        /* Fallback signature identifier for newer A10X secureROM/iBoot variants */
        ldr_insn = find_next_ARM64_literal_reference(iboot_in->buf, iboot_in->len, "IMAGE", 5);
        if (!ldr_insn) {
            return NULL;
        }
    }

    return find_bl_verify_shsh_insn_64(iboot_in, ldr_insn);
}

void* find_bl_verify_shsh_generic_64(struct iboot_img* iboot_in) {
    void* ldr_insn = find_next_ARM64_literal_reference(iboot_in->buf, iboot_in->len, "RT", 2);
    if (!ldr_insn) {
        return NULL;
    }

    return find_bl_verify_shsh_insn_64(iboot_in, ldr_insn);
}

void* find_bl_verify_shsh_insn_64(struct iboot_img* iboot_in, void* pc) {
    /* Find the top of the function via ARM64 frame pointer analysis (stp x29, x30, [sp, ...]) */
    void* function_top = find_verify_shsh_top_64(pc);
    if (!function_top) {
        return NULL;
    }

    /* Locate the BL instruction branching to this verification routine */
    uint32_t target_offset = (uint32_t)((uintptr_t)GET_IBOOT_FILE_OFFSET(iboot_in, function_top));
    void* bl_verify_shsh = find_next_bl_insn_to_64(iboot_in->buf, iboot_in->len, target_offset);
    if (!bl_verify_shsh) {
        return NULL;
    }

    return bl_verify_shsh;
}

void* find_verify_shsh_top_64(void* ptr) {
    /* Traverse upwards in memory to find the ARM64 stack frame setup prologue: 
     * e.g., stp x29, x30, [sp, #-0x20]! or similar stack allocation */
    void* top = arm64_frame_prologue_search_up(ptr, 0x600);
    return top;
}
