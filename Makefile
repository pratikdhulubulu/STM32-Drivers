# Toolchain Setup
CC       = arm-none-eabi-gcc
AS       = arm-none-eabi-as
OBJCOPY  = arm-none-eabi-objcopy
OBJDUMP  = arm-none-eabi-objdump
SIZE     = arm-none-eabi-size
GDB      = arm-none-eabi-gdb
RM       = rm -rf

# MCU Configuration
MCU       = cortex-m4
FPU       = fpv4-sp-d16
FLOAT-ABI = hard

# Project Structure
SRC_ROOT  = Applications
DRIVERS   = Drivers
UTILS     = Utilities
CORE      = Core
STARTUP   = $(CORE)/Startup
LD_SCRIPT = Linker/linker_script.ld

# Build Versioning
VERSION     = MY_RELEASE_1.0.2
BUILD_DATE := $(shell date +"%Y-%m-%d")
BUILD_TIME := $(shell date +"%H:%M:%S")

CFLAGS += -DBUILD_DATE=\"$(BUILD_DATE)\"
CFLAGS += -DBUILD_TIME=\"$(BUILD_TIME)\"
CFLAGS += -DBUILD_VERSION=\"$(VERSION)\"

# Build Configuration
ifeq ($(DEBUG),1)
CFLAGS_OPT = -O0 -g
BUILD_TYPE = Debug
else ifeq ($(RELEASE),1)
CFLAGS_OPT = -O2
BUILD_TYPE = Release
else
CFLAGS_OPT = -O0 -g
BUILD_TYPE = Default(Debug)
endif

# Selected Application
APP ?= GPIO_Toggle
BUILD_DIR = Build/$(APP)

# Source File Discovery
SRCS := $(wildcard $(SRC_ROOT)/$(APP)/*.c) \
        $(foreach d,$(wildcard $(DRIVERS)/*),$(wildcard $(d)/*.c)) \
        $(wildcard $(UTILS)/*.c) \
        $(wildcard $(CORE)/*.c) \
        $(wildcard $(CORE)/*/*.c)

ASMS := $(wildcard $(STARTUP)/*.s)

# Object File Destination
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS)) \
        $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASMS))

# Include Directories
INC_DIRS = \
    $(SRC_ROOT)/$(APP) \
    $(UTILS) \
    $(CORE) \
    $(wildcard $(DRIVERS)/*)

CFLAGS  += -Wall $(CFLAGS_OPT) -std=gnu11
CFLAGS  += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)
CFLAGS  += $(foreach dir,$(INC_DIRS),-I$(dir))

LDFLAGS = -T$(LD_SCRIPT) -nostartfiles
LDFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)

# Primary Targets
.PHONY: all clean flash debug info kill_openocd

all: $(BUILD_DIR)/$(APP).elf $(BUILD_DIR)/$(APP).bin $(BUILD_DIR)/$(APP).hex
	@echo "---------------------------------------------------------"
	@echo "Build complete - App: $(APP) | Mode: $(BUILD_TYPE)"
	@echo "Build Version: $(VERSION)"
	@echo "Build Date: $(BUILD_DATE)  Time: $(BUILD_TIME)"
	@echo "---------------------------------------------------------"

# Compile C sources
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "Assembling $<"
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@

# Create Build folder
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Linking
$(BUILD_DIR)/$(APP).elf: $(OBJS)
	@echo "Linking..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Created ELF: $@"
	@$(SIZE) --format=berkeley $@
	@$(OBJDUMP) -h -S $@ > $(BUILD_DIR)/$(APP)_sections.txt
	@echo "Section details : $(BUILD_DIR)/$(APP)_sections.txt"

# Generate BIN / HEX
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O binary $< $@
	@echo "Created binary  : $@"

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O ihex $< $@
	@echo "Created hex     : $@"

# Flashing
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program $(BUILD_DIR)/$(APP).elf verify reset exit"

# Debugging
debug: all
	@echo "Starting OpenOCD..."
	@nohup openocd -f interface/stlink.cfg \
	               -c "transport select swd" \
	               -f target/stm32f4x.cfg \
	               > $(BUILD_DIR)/openocd.log 2>&1 &

	@sleep 2

	@echo "Launching GDB..."
	$(GDB) $(BUILD_DIR)/$(APP).elf \
		-ex "target extended-remote localhost:3333" \
		-ex "monitor reset halt" \
		-ex "load" \
		-ex "monitor reset halt" \
		-ex "set pagination off"

kill_openocd:
	@pkill -f openocd

# Clean build folder
clean:
	@echo "Cleaning build files..."
	@$(RM) $(BUILD_DIR)
	@echo "Clean done."

# Build info
info:
	@echo "Selected App : $(APP)"
	@echo "Source files : $(SRCS)"
	@echo "Object files : $(OBJS)"
	@echo "Include path : $(INC_DIRS)"
	@echo "Build Mode   : $(BUILD_TYPE)"
	@echo "Build Date   : $(BUILD_DATE) $(BUILD_TIME)"
	@echo "Version Tag  : $(VERSION)"
