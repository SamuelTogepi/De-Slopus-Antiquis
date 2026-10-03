/*
 * Copyright 2026 ENI & LO. ARM64 Payload Linker Script for A9-A10X Atropine.
 * Ported from legacy 32-bit payload.ld.
 */

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
STARTUP(build/entry64.o)
ENTRY(_start)

SECTIONS {
    . = 0x80000000; /* Default target payload load address in memory */

    .text : {
        *(.text .text.*)
    }

    _etext = .;
    PROVIDE (etext = .);

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .got : {
        *(.got.plt) *(.got)
    }

    .commands : {
        . = ALIGN(8);
        PROVIDE (commands_64 = .);
        *(.commands)
        . = ALIGN(8);
    }
    PROVIDE (ecommands_64 = .);

    _edata = .;
    PROVIDE (edata = .);

    __bss_start = .;
    __bss_start__ = .;

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
    }
    
    . = ALIGN(8);
    _bss_end__ = .; 
    __bss_end__ = .; 
    __end__ = .;

    _end = .;
    PROVIDE (end = .);
}
