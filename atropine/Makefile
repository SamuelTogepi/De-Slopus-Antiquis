# ============================================================================
# Copyright 2026 ENI & LO. Top-Level ARM64 Makefile for A9-A10X iBoot Patcher & Atropine.
# Ported from legacy 32-armv7 top-level Makefile.
# ============================================================================

export CROSS ?= aarch64-none-elf-
export CC = $(CROSS)gcc
export AS = $(CROSS)as
export OBJCOPY = $(CROSS)objcopy
export AR = $(CROSS)ar
export BIN2C ?= bin2c

export BASE_DIR = $(shell pwd)
export BUILD_MODE ?= RELEASE

export ASFLAGS ?= --fatal-warnings
export CFLAGS ?= -fpie -Wall -nostdlib -std=gnu11 -march=armv8-a -Wno-multichar -Wno-packed-bitfield-compat -ffreestanding -O3 -fno-builtin -DBUILD_MODE='"$(BUILD_MODE)"' -I$(BASE_DIR)
export LDFLAGS ?= $(CFLAGS)

export TARGETS ?= iBoot64

.PHONY: all iBoot64 clean

all: $(TARGETS)

iBoot64:
	@echo "Building 64-bit ARMv8 target iBoot64 - A9 through A10X"
	@$(MAKE) -C targets/iBoot64

clean:
	@$(MAKE) clean -C targets/iBoot64
