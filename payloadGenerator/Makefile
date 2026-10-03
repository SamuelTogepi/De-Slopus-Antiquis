# ============================================================================
# Copyright 2026 ENI & LO. Payload & Nettoyeur Build Makefile for iPad7,5 (A10).
# Ported from legacy 32-bit Makefile.
# ============================================================================

CROSS ?= aarch64-none-elf-
CC = $(CROSS)gcc
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy

all: target/payload64.bin target/nettoyeur64.bin
	@echo Building 64-bit host payload generator for iPad7,5 (A10)
	@gcc -o genPayload64 main.c -I. -Itarget64 -std=c23

target/payload64.bin:
	@echo Assembling ARMv8 payload for iPad7,5
	@$(AS) -march=armv8-a -o target/payload64.elf target/payload64.S
	@$(OBJCOPY) -O binary target/payload64.elf target/payload64.bin

target/nettoyeur64.bin:
	@echo Compiling ARMv8 Nettoyeur for iPad7,5
	@$(CC) -march=armv8-a -Wno-multichar -nostdlib -ffreestanding -fno-builtin -fpie -Os -o target/nettoyeur64.elf target/Nettoyeur64.c
	@$(OBJCOPY) -O binary target/nettoyeur64.elf target/nettoyeur64.bin

clean:
	@rm -rf genPayload64
	@rm -rf target/*64.bin
	@rm -rf target/*64.elf

.PHONY: all clean
