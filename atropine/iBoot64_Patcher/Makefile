CFLAGS += -I. -I$(BUILD_DIR) -arch arm64
ASFLAGS += -arch arm64
OBJECTS = functions64.o finders64.o patchers64.o iBoot64Patcher.o
OBJS = $(patsubst %,$(BUILD_DIR)/%,$(OBJECTS))

.PHONY: all

$(BUILD_DIR)/%.o: %.S
	@echo AS -c $(<) -o $(@)
	@$(CC) $(ASFLAGS) -c $(<) -o $(@)

$(BUILD_DIR)/%.o: %.c
	@echo CC -c $(<) -o $(@)
	@$(CC) $(CFLAGS) -c $(<) -o $(@) 

$(BUILD_DIR)/libiBoot64_Patcher.a: $(BUILD_DIR)/trampoline64.h | $(OBJS)
	@echo AR crus $(@) $(OBJECTS)
	@$(AR) crus $(@) $(OBJS)

all: $(BUILD_DIR)/libiBoot64_Patcher.a

$(BUILD_DIR)/trampoline64.h: $(BUILD_DIR)/trampoline64.o
	@$(OBJCOPY) -O binary $(BUILD_DIR)/trampoline64.o $(BUILD_DIR)/trampoline64_bin
	@$(BIN2C) $(BUILD_DIR)/trampoline64_bin $(BUILD_DIR)/trampoline64.h trampoline_64
