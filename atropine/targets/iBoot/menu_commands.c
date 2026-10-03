/*
 * Copyright 2026 ENI & LO. ARM64 Core Command Implementations for A9-A10X Atropine.
 * Ported from legacy 32-bit commands.c.
 */

#include <command64.h>
#include <address64.h>
#include <display/display.h>
#include <libc/libc.h>

extern uintptr_t end;
static const char* rd_boot_args = "rd=md0";

int fsboot_cmd(int argc, cmd_arg_t* argv) {
    /* Sets the kernel boot-args pointer to those in the 'boot-args' environment variable */
    *(uintptr_t*)boot_args_ref = (uintptr_t)get_env("boot-args");
    clear_icache_64();

    printf("Booting system\n");
    display_progress_print("Booting system...");
    display_progress_bar(100);
    
    fsboot();
    return 0;
}

ADD_COMMAND("fsboot", fsboot_cmd, "Mounts the system rootfs partition and executes the kernelcache");

int ramdisk_cmd(int argc, cmd_arg_t* argv) {
    *ramdisk_size_ptr = (size_t)get_env_uint("filesize");
    if (!*ramdisk_size_ptr) {
        /* Default ramdisk size of 30MB */
        *ramdisk_size_ptr = 30000000;
    }

    /* We use the end of the payload memory section as the base address to place our ramdisk */
    *ramdisk_address_ptr = (uintptr_t)&end;

    memcpy((void*)*ramdisk_address_ptr, loadaddr, *ramdisk_size_ptr);
    return 0;
}

ADD_COMMAND("ramdisk", ramdisk_cmd, "Load a raw ramdisk to boot");

int rdboot_cmd(int argc, cmd_arg_t* argv) {
    /* Verify a ramdisk is loaded and allocated */
    if (!*ramdisk_size_ptr || !*ramdisk_address_ptr) {
        printf("No ramdisk loaded\n");
        return -1;
    }

    /* Reset the boot-args to our standard default ramdisk boot string */
    *(uintptr_t*)boot_args_ref = (uintptr_t)rd_boot_args;
    clear_icache_64();

    printf("Booting system with ramdisk\n");
    display_progress_print("Booting system...");
    display_progress_bar(100);

    fsboot();
    return 0;
}

ADD_COMMAND("rdboot", rdboot_cmd, "Boot a ramdisk loaded previously");

int echo_cmd(int argc, cmd_arg_t* argv) {
    for (int i = 0; i < argc; i += 1) {
        printf("%s ", argv[i].string);
    }
    printf("\n");
    return 0;
}

ADD_COMMAND("echo", echo_cmd, "Repeats stdin to stdout");

int diff_cmd(int argc, cmd_arg_t* argv) {
    uint32_t iboot_end = get_env_uint("filesize");
    for (uint32_t i = 0; i <= iboot_end; i += 4) {
        uint32_t tru = *(uint32_t*)(((uintptr_t)loadaddr) + i);
        uint32_t pru = *(uint32_t*)(((uintptr_t)base_address) + i);
        if (tru != pru) {
            printf("*(uint32_t*)(BASE_ADDRESS + 0x%x) = 0x%x; /* 0x%x -> 0x%x */\n", i, tru, pru, tru);
        }
    }
    return 0;
}

ADD_COMMAND("diff", diff_cmd, "Diff iBoot with an uploaded iBoot binary");

int help_cmd(int argc, cmd_arg_t* argv) {
    printf("Available commands (ARM64):\n");
    cmd_handler_t* cmd = (cmd_handler_t*)&commands_64;

    /* Iterate through the 64-bit commands table section */
    while (cmd != (cmd_handler_t*)&ecommands_64) {
        if (cmd->name && cmd->description) {
            printf("\t%s\t%s\n", cmd->name, cmd->description);
        }
        cmd += 1;
    }
    return 0;
}

ADD_COMMAND("help", help_cmd, "Prints this help dialog");
