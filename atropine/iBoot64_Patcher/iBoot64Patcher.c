/*
 * Copyright 2026 ENI & LO. Main ARM64 iBoot Patcher Orchestrator for A9-A10X.
 * Ported from legacy ARM32 iBoot32Patcher core driver.
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

int patch_iboot_64(void* buf, size_t len) {
    int ret = 0;
    struct iboot_img iboot_in;
    iboot_in.buf = buf;
    iboot_in.len = len;

    /* In ARM64 iBoot binaries (A9-A10X), the version string offset layout 
     * shifts due to 64-bit header padding and Mach-O segment headers. 
     * We locate the version string dynamically or via 64-bit offset mapping. */
    char* iboot_vers_str = arm64_find_iboot_version_string(iboot_in.buf, iboot_in.len);
    if (!iboot_vers_str) {
        // Fallback to standard 64-bit offset definition if dynamic scan misses
        iboot_vers_str = (char*)iboot_in.buf + IBOOT_VERS_STR_OFFSET_64;
    }

    iboot_in.VERS = atoi(iboot_vers_str);

    /* Patch RSA signature check / ticket validation for blobless downgrades */
    ret = patch_rsa_check_64(&iboot_in);
    if (ret != 0) {
        return -1;
    }

#ifdef BUILD_SECUREROM
    /* LLB / iBSS loader payload enforcement patches */
    patch_llb_load_64(&iboot_in);

    ret = patch_command_handler_64(&iboot_in);
    if (ret != 0) {
        return -1;
    }
#endif

#ifdef BUILD_IBOOT
    /* Unlock boot arguments for custom device tree boot-args injection */
    ret = patch_boot_args_64(&iboot_in);
    if (ret != 0) {
        return -1;
    }
#endif

#ifdef BUILD_IBEC
    /* Bypass ticket and SHSH manifest checks in iBEC for kernel handoff */
    ret = patch_ticket_check_64(&iboot_in);
    if (ret != 0) {
        return -1;
    }
#endif

    return 0;
}
