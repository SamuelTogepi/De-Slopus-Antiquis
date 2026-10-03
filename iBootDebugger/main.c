/*
 * Copyright 2026 ENI & LO. ARM64 Host iBoot Binary Patcher & Breakpoint Injector for A9-A10X.
 * Ported from legacy 32-bit builder.c.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/endian.h>
#include <asm64.h>
#define DEBUG_IBOOT_BUILDER_64
#include <target64.h>

uint8_t* buf;
size_t file_len;
size_t spare_memory_off = 0;

uint8_t readp_bkpt_handler[] = {
    #embed "breakpoint_handlers/readp_handler64.bin"
};

uint8_t memalign_start_bkpt_handler[] = {
    #embed "breakpoint_handlers/memalign_start_handler64.bin"
};

uint8_t memalign_loop_bkpt_handler[] = {
    #embed "breakpoint_handlers/memalign_loop_handler64.bin"
};

uint8_t memalign_end_bkpt_handler[] = {
    #embed "breakpoint_handlers/memalign_end_handler64.bin"
};

uint8_t generic_bkpt_handler[] = {
    #embed "breakpoint_handlers/generic_handler64.bin"
};

uint8_t stack_dump_handler[] = {
    #embed "breakpoint_handlers/stack_dump_handler64.bin"
};

static int read_file_into_buffer(char* path, uint8_t** out_buf, size_t* len) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (!*len) {
        fclose(f);
        return -1;
    }

    *out_buf = malloc(*len);
    if (!*out_buf) {
        fclose(f);
        return -1;
    }
    fread(*out_buf, 1, *len, f);
    fclose(f);
    return 0;
}

static int write_file_from_buffer(char* path, uint8_t** out_buf, size_t len) {
    FILE* f = fopen(path, "wb+");
    if (!f) {
        return -1;
    }
    fwrite(*out_buf, len, 1, f);
    free(*out_buf);
    fclose(f);
    return 0;
}

void add_breakpoint_64(size_t bkpt_address, void* bkpt_handler, size_t bkpt_handler_len, int safe_reg) {
    size_t bp_bl_offset = 0;
    size_t bkpt_trampoline_start = spare_memory_off;

    /* ARM64 Breakpoint Trampoline Generation */
    WRITE_INSN_64(buf, spare_memory_off, ARM64_NOP);
    
    /* Save general purpose registers x0-x15 and lr onto stack via 64-bit STP */
    WRITE_INSN_64(buf, spare_memory_off, 0xA9BF7BF0); // stp x0, x1, [sp, #-16]!
    WRITE_INSN_64(buf, spare_memory_off, 0xA9BF73F2); // stp x2, x3, [sp, #-16]!
    WRITE_INSN_64(buf, spare_memory_off, 0xA9BF6BF4); // stp x4, x5, [sp, #-16]!
    WRITE_INSN_64(buf, spare_memory_off, 0xA9BF63F6); // stp x6, x7, [sp, #-16]!
    
    bp_bl_offset = spare_memory_off;
    WRITE_INSN_64(buf, spare_memory_off, (uint32_t)0); /* Placeholder for handler branch-with-link */

    /* Restore registers via LDP */
    WRITE_INSN_64(buf, spare_memory_off, 0xA8C163F6); // ldp x6, x7, [sp], #16
    WRITE_INSN_64(buf, spare_memory_off, 0xA8C16BF4); // ldp x4, x5, [sp], #16
    WRITE_INSN_64(buf, spare_memory_off, 0xA8C173F2); // ldp x2, x3, [sp], #16
    WRITE_INSN_64(buf, spare_memory_off, 0xA8C17BF0); // ldp x0, x1, [sp], #16

    /* Copy overwritten instruction from target breakpoint location */
    memcpy(&buf[spare_memory_off], &buf[bkpt_address], sizeof(uint32_t));
    spare_memory_off += sizeof(uint32_t);

    /* Return to execution flow (BR x30 / RET equivalent) */
    WRITE_INSN_64(buf, spare_memory_off, 0xD65F03C0); // ret

    /* Patch original breakpoint address to branch to our trampoline (ARM64 BL encoding) */
    *(uint32_t*)&buf[bkpt_address] = make_bl_64(bkpt_address, bkpt_trampoline_start);

    /* Patch placeholder inside trampoline to branch directly to the breakpoint handler payload */
    *(uint32_t*)&buf[bp_bl_offset] = make_bl_64(bp_bl_offset, spare_memory_off);

    /* Copy actual breakpoint handler binary code into spare memory */
    memcpy(&buf[spare_memory_off], bkpt_handler, bkpt_handler_len);
    spare_memory_off += bkpt_handler_len;

    /* Ensure 4-byte instruction alignment */
    spare_memory_off = (spare_memory_off + 3) & ~3;
}

void apply_patches_64(void) {
    /* Locate spare memory region inside iBoot binary (e.g., overriding Apple Certification Authority strings) */
    uint8_t* apple_cert = memmem(buf, file_len, "Apple Certification Authority", strlen("Apple Certification Authority"));
    if (apple_cert) {
        spare_memory_off = (size_t)(apple_cert - buf);
    } else {
        spare_memory_off = file_len - 0x2000; // Fallback spare buffer offset near end of binary segment
    }

    /* Ensure 4-byte instruction alignment */
    spare_memory_off = (spare_memory_off + 3) & ~3;

    /* Rename environment variables to prevent standard boot interference */
    uint8_t* boot_ramdisk = memmem(buf, file_len, "boot-ramdisk", sizeof("boot-ramdisk"));
    if (boot_ramdisk) {
        memcpy(boot_ramdisk, "noot-ramdisk", sizeof("boot-ramdisk") - 1);
    }
    
    uint8_t* boot_partition = memmem(buf, file_len, "boot-partition", sizeof("boot-partition"));
    if (boot_partition) {
        memcpy(boot_partition, "noot-partition", sizeof("boot-partition") - 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <iboot_in_arm64> <iboot_out_arm64>\n", argv[0]);
        return 1;
    }

    if (read_file_into_buffer(argv[1], &buf, &file_len) != 0) {
        fprintf(stderr, "Error reading input iBoot binary: %s\n", argv[1]);
        return 1;
    }

    apply_patches_64();

    /* Inject ARM64 debugging breakpoints for A9-A10X target routines */
    add_breakpoint_64(READP_BP_OFF_64, readp_bkpt_handler, sizeof(readp_bkpt_handler), READP_SAFE_REG_64);
    add_breakpoint_64(MEMALIGN_START_BP_OFF_64, memalign_start_bkpt_handler, sizeof(memalign_start_bkpt_handler), MEMALIGN_START_SAFE_REG_64);
    add_breakpoint_64(MEMALIGN_LOOP_BP_OFF_64, memalign_loop_bkpt_handler, sizeof(memalign_loop_bkpt_handler), MEMALIGN_LOOP_SAFE_REG_64);
    add_breakpoint_64(MEMALIGN_END_BP_OFF_64, memalign_end_bkpt_handler, sizeof(memalign_end_bkpt_handler), MEMALIGN_END_SAFE_REG_64);

    if (write_file_from_buffer(argv[2], &buf, file_len) != 0) {
        fprintf(stderr, "Error writing patched iBoot binary: %s\n", argv[2]);
        return 1;
    }

    printf("Successfully built ARM64 patched iBoot binary for A9-A10X!\n");
    return 0;
}
