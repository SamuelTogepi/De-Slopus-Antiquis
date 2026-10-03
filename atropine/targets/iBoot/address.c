/*
 * Copyright 2026 ENI & LO. ARM64 iBoot Exploit Symbol Finder for A9-A10X.
 * Ported from legacy 32-bit iBoot address resolution engine.
 */

#include <address64.h>
#include <iBoot64_Patcher/include/functions64.h>

uintptr_t* base_address;
printf_t printf;
get_env_uint_t get_env_uint;
get_env_t get_env;
uintptr_t* framebuffer_address;
uintptr_t* display_timings_address;
uint32_t display_width;
uint32_t display_height;
char* boot_args_ref;
fsboot_t fsboot;
jumpto_t jumpto;
uintptr_t* ramdisk_address_ptr;
size_t* ramdisk_size_ptr;
void* loadaddr;

#define GET_ENV_UINT_SEARCH "bootdelay"
#define GET_ENV_SEARCH "boot-command"
#define PRINTF_SEARCH "Memory image not valid\n"
#define FRAMEBUFFER_ADDRESS_SEARCH "framebuffer"
#define DISPLAY_TIMING_SEARCH "display-timing"
#define BOOTARGS_SEARCH "rd=md0 nand-enable-reformat=1 -progress"
#define FSBOOT_SEARCH "fsboot"
#define JUMPTO_SEARCH "Memory image not valid\n"
#define LOADADDR_SEARCH "loadaddr"

int find_functions_64(void* iboot_buf, size_t iboot_len) {
    base_address = (uintptr_t*)iboot_buf;
    if (!base_address) {
        return -1;
    }

    printf = find_printf_64(iboot_len);
    if (!printf) {
        return -1;
    }

    get_env_uint = find_get_env_uint_64(iboot_len);
    if (!get_env_uint) {
        return -1;
    }

    get_env = find_get_env_64(iboot_len);
    if (!get_env) {
        return -1;
    }

    framebuffer_address = (uintptr_t*)find_framebuffer_address_64();
    display_timings_address = (uintptr_t*)find_display_timings_address_64(iboot_len);
    
    if (display_timings_address) {
        display_width = (uint32_t)display_timings_address[4];
        display_height = (uint32_t)display_timings_address[8];
    }

    boot_args_ref = find_boot_args_ref_64(iboot_len);
    if (!boot_args_ref) {
        return -1;
    }

    fsboot = find_fsboot_64(iboot_len);
    jumpto = find_jumpto_func_64(iboot_len);
    loadaddr = find_load_address_64();

    return 0;
}

void* quick_find_64(size_t iboot_len, const char* str, size_t len) {
    void* ref = memmem(base_address, iboot_len, str, len);
    if (!ref) {
        return NULL;
    }

    void* xref = memmem(base_address, iboot_len, &ref, sizeof(ref));
    if (!xref) {
        return NULL;
    }

    void* ldr = ldr_to_64_struct_scan(base_address, iboot_len, xref);
    if (!ldr) {
        return NULL;
    }

    void* bl = bl_search_down_64(ldr, 0x20);
    if (!bl) {
        return NULL;
    }

    return resolve_bl64(bl);
}

void* last_xref_64(size_t iboot_len, const char* pattern, size_t len) {
    uintptr_t* ref = memmem(base_address, iboot_len, pattern, len);
    if (!ref) {
        return NULL;
    }

    uintptr_t* curr_xref = memmem(base_address, iboot_len, &ref, sizeof(ref));
    if (!curr_xref) {
        return NULL;
    }
    uintptr_t* prev_xref = curr_xref;

    while (curr_xref != NULL) {
        prev_xref = curr_xref;
        curr_xref = memmem((char*)curr_xref + 0x8, iboot_len - ((char*)curr_xref - (char*)base_address + 0x8), &ref, sizeof(ref));
    }
    return prev_xref;
}

printf_t find_printf_64(size_t iboot_len) {
    return (printf_t)quick_find_64(iboot_len, PRINTF_SEARCH, sizeof(PRINTF_SEARCH));
}

get_env_uint_t find_get_env_uint_64(size_t iboot_len) {
    return (get_env_uint_t)quick_find_64(iboot_len, GET_ENV_UINT_SEARCH, sizeof(GET_ENV_UINT_SEARCH));
}

get_env_t find_get_env_64(size_t iboot_len) {
    return (get_env_t)quick_find_64(iboot_len, GET_ENV_SEARCH, sizeof(GET_ENV_SEARCH));
}

uintptr_t* find_framebuffer_address_64(void) {
    return (uintptr_t*)get_env_uint(FRAMEBUFFER_ADDRESS_SEARCH);
}

uintptr_t* find_display_timings_address_64(size_t iboot_len) {
    char* type = get_env(DISPLAY_TIMING_SEARCH);
    if (!type) {
        return NULL;
    }
    return last_xref_64(iboot_len, type, strlen(type) + 1);
}

char* find_boot_args_ref_64(size_t iboot_len) {
    uintptr_t* ref = memmem(base_address, iboot_len, BOOTARGS_SEARCH, sizeof(BOOTARGS_SEARCH));
    if (!ref) {
        return NULL;
    }
    return memmem(base_address, iboot_len, &ref, sizeof(ref));
}

fsboot_t find_fsboot_64(size_t iboot_len) {
    uintptr_t* xref = last_xref_64(iboot_len, FSBOOT_SEARCH, sizeof(FSBOOT_SEARCH));
    if (!xref) {
        return NULL;
    }
    return (fsboot_t)xref[1];
}

jumpto_t find_jumpto_func_64(size_t iboot_len) {
    void* ref = memmem(base_address, iboot_len, JUMPTO_SEARCH, sizeof(JUMPTO_SEARCH));
    if (!ref) {
        return NULL;
    }

    void* xref = memmem(base_address, iboot_len, &ref, sizeof(uintptr_t));
    if (!xref) {
        return NULL;
    }

    void* ldr = ldr_to_64_struct_scan(base_address, iboot_len, xref);
    if (!ldr) {
        return NULL;
    }

    void* bl = bl_search_down_64(ldr, 0x100);
    if (bl) {
        return resolve_bl64(bl);
    }

    return NULL;
}

void* find_load_address_64(void) {
    return (void*)get_env_uint(LOADADDR_SEARCH);
}
