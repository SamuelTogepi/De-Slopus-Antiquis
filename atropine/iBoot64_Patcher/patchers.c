/*
 * Copyright 2026 ENI & LO. ARM64 Patcher Implementation for A9-A10X.
 * Ported from legacy ARM32 iBoot32Patcher patchers.c codebase.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <include/arm64_defs.h>
#include <include/finders64.h>
#include <include/functions64.h>
#include <include/iBoot64Patcher.h>
#include <include/patchers64.h>
#include <trampoline64.h>

/* Force-enable custom boot arguments by bypassing conditional checks in ARM64 iBoot */
int patch_boot_args_64(struct iboot_img* iboot_in) {
    void* default_boot_args_str_loc = memstr(iboot_in->buf, iboot_in->len, DEFAULT_BOOTARGS_STR);
    if (!default_boot_args_str_loc) {
        return -1;
    }

    void* default_boot_args_xref = iboot_memmem_64(iboot_in, default_boot_args_str_loc);
    if (!default_boot_args_xref) {
        return -1;
    }

    /* In ARM64, locate the instruction referencing the default boot args string (via ADRP/ADD or LDR literal) */
    void* ldr_boot_args = ldr_to_64(iboot_in, default_boot_args_xref);
    if (!ldr_boot_args) {
        return -1;
    }

    /* Patch conditional branches or zero comparisons following bootargs evaluation to enforce active inclusion */
    uint32_t* insn_ptr = (uint32_t*)ldr_boot_args;
    for (int i = 0; i < 16; i++) {
        // Look for CBNZ / CBZ or conditional branches (e.g., B.NE / B.EQ) and neutralize them to NOP
        uint32_t op = insn_ptr[i];
        if ((op & 0xFC000000) == 0x54000000) { // Conditional branch (B.cond)
            insn_ptr[i] = ARM64_NOP; // NOP out conditional check
            break;
        }
    }

    return 0;
}

/* Patches LLB / iBSS to load an image with tag 'ibox' instead of 'ibot' */
int patch_llb_load_64(struct iboot_img* iboot_in) {
    int os_vers = get_os_version_64(iboot_in);

    if (os_vers >= 9 && os_vers <= 12) {
        /* Search for the immediate tag value 'ibot' (0x746F6269) and patch to 'ibox' (0x786F6269) */
        uint32_t ibot_tag = 'ibot';
        uint32_t* ibot_tag_ptr = memmem(iboot_in->buf, iboot_in->len, &ibot_tag, sizeof(uint32_t));
        if (!ibot_tag_ptr) {
            return -1;
        }
        *ibot_tag_ptr = 'ibox';
        return 0;
    }

    uint32_t ibot_tag = 'ibot';
    uint32_t* ibot_tag_ptr = memmem(iboot_in->buf, iboot_in->len, &ibot_tag, sizeof(uint32_t));
    if (!ibot_tag_ptr) {
        return -1;
    }
    *ibot_tag_ptr = 'ibox';
    return 0;
}

/* Bypass RSA signature and SHSH ticket validation checks for blobless boots */
int patch_rsa_check_64(struct iboot_img* iboot_in) {
    void* bl_verify_shsh = find_bl_verify_shsh_64(iboot_in);
    if (!bl_verify_shsh) {
        return -1;
    }

    /* In ARM64, replace the verify_shsh function call entry with:
     * MOV x0, #0 (Return success / 0)
     * RET        (Return from function immediately) */
    uint32_t* patch_ptr = (uint32_t*)bl_verify_shsh;
    patch_ptr[0] = 0xD2800000; // MOV x0, #0
    patch_ptr[1] = 0xD65F03C0; // RET

    return 0;
}

/* Injects command handler trampoline for custom command execution in iBSS/iBoot */
int patch_command_handler_64(struct iboot_img* iboot_in) {
    /* Copy the ARM64 iBSS trampoline to designated free space region */
    char* trampoline_dest = (char*)iboot_in->buf + TRAMPOLINE_OFFSET_64;
    memcpy(trampoline_dest, trampoline_64, trampoline_length_64);
    
    void* jumpto = find_jumpto_64(iboot_in);
    if (!jumpto) {
        return -1;
    }

    uint64_t jumpto_addr = (uint64_t)GET_IBOOT_ADDR(iboot_in, jumpto);
    char* jumpto_addr_offset = trampoline_dest + JUMPTO_ADDR_OFFSET_64;
    *(uint64_t*)jumpto_addr_offset = jumpto_addr;

    /* Replace jumpto calls with branches to our custom trampoline */
    void* jumpto_call = find_next_bl_insn_to_64(iboot_in, (uint32_t)((uintptr_t)GET_IBOOT_FILE_OFFSET(iboot_in, jumpto)));
    while (jumpto_call) {
        build_bl_64(jumpto_call, trampoline_dest);
        jumpto_call = find_next_bl_insn_to_64(iboot_in, (uint32_t)((uintptr_t)GET_IBOOT_FILE_OFFSET(iboot_in, jumpto)));
    }

    return 0;
}

/* Bypass strict ticket verification checks in iBEC for untethered downgrades */
int patch_ticket_check_64(struct iboot_img* iboot_in) {
    /* Locate iBoot version string reference in ARM64 binary image */
    const char* iboot_vers_str = memstr(iboot_in->buf, iboot_in->len, "iBoot-");
    if (!iboot_vers_str) {
        return -1;
    }

    /* Locate the target verification routine block and patch out error returns */
    void* ticket_check_func = find_ticket_check_routine_64(iboot_in);
    if (!ticket_check_func) {
        return -1;
    }

    uint32_t* code_ptr = (uint32_t*)ticket_check_func;
    
    /* Force success return code (MOV w0, #0; RET) inside ticket validation routine */
    code_ptr[0] = 0x52800000; // MOV w0, #0
    code_ptr[1] = 0xD65F03C0; // RET

    return 0;
}
