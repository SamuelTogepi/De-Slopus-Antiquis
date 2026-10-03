/*
 * Copyright 2026 ENI & LO. ARM64 Atropine Payload Main Controller for A9-A10X.
 * Ported from legacy 32-bit Atropine main.c.
 */

#include <libc/libc.h>
#include <address64.h>
#include <display/display.h>
#include <command64.h>
#include <iBoot64_Patcher/include/iBoot64Patcher.h>
#include <handler64.h>

int init = 0;

int atropine_init(void* iboot_buf, size_t iboot_len) {
    int ret;

    /* First, locate all required symbols and function pointers in the 64-bit iBoot image */
    ret = find_functions_64(iboot_buf, iboot_len);
    if (ret != 0) {
        return -1;
    }

    /* Initialize the screen display with the located framebuffer address, width, and height */
    display_init((void*)framebuffer_address, display_width, display_height);

    display_progress_print("Patching iBoot (ARM64)");
    printf("Patching iBoot (ARM64 A9-A10X)\n");

    display_progress_bar(20);

    printf("Fixing command handler\n");
    display_progress_bar(40);

    /* Relocate and rename the command handler structure for our custom shell hook */
    ret = fix_cmd_handler_64(iboot_buf, iboot_len);
    if (ret != 0) {
        return ret;
    }

    printf("Hooking jumpto\n");
    display_progress_bar(60);

    /* Clear the instruction cache using ARM64 system instructions to ensure cache coherency */
    clear_icache_64();
    
    printf("Ready to boot\n");
    display_progress_print("Ready to boot");
    display_progress_bar(80);

    return 0;
}

int main(int argc, cmd_arg_t* argv) {
    if (!init) {
        init = 1;
        /* Pass the active iBoot buffer context (stored via base_address) to initialization */
        return atropine_init(base_address, IBOOT_LEN_64);
    }
    if (argc <= 1) {
        return 0;
    }
    return iterate_commands_64(argc - 1, &argv[1]);
}
