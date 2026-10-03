# ============================================================================
# Copyright 2026 ENI & LO. Host Breakpoint Builder Makefile for iPad7,5 (A10).
# Ported from legacy host Makefile.
# ============================================================================

all: breakpoint_handlers genDebug64

genDebug64:
	@echo Building 64-bit host debug builder for iPad7,5 (A10)
	@gcc -o genDebug64 main.c -Itarget64 -I. -std=c23

breakpoint_handlers:
	@echo Building breakpoint handlers for ARM64 A10
	@$(MAKE) -C breakpoint_handlers

clean:
	@rm -rf genDebug64
	@$(MAKE) clean -C breakpoint_handlers

.PHONY: breakpoint_handlers genDebug64
