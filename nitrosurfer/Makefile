#---------------------------------------------------------------------------------
# Makefile for Nitro Surfer GBA
#---------------------------------------------------------------------------------

DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM

CC      := $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY := $(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX  := $(DEVKITPRO)/tools/bin/gbafix

TARGET  := nitrosurfer
BUILD   := build
SRC_DIR := source
INC_DIR := include

ARCH    := -mthumb -mthumb-interwork -mcpu=arm7tdmi
CFLAGS  := $(ARCH) -O2 -Wall -Wextra -fomit-frame-pointer \
           -I$(DEVKITPRO)/libtonc/include -I$(INC_DIR)
LDFLAGS := $(ARCH) -specs=gba.specs -L$(DEVKITPRO)/libtonc/lib -ltonc

C_SRCS  := $(wildcard $(SRC_DIR)/*.c)
OBJS    := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(C_SRCS))

.PHONY: all clean

all: $(TARGET).gba

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	@echo "  LD    $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
$(TARGET).gba: $(TARGET).elf
	@echo "  OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@
	@echo "  GBAFIX  $@"
	@$(GBAFIX) $@ -t"NITRO SURFER" -cNITR -m00 -r1
	@cp $@ ../nitrosurfer.gba 2>/dev/null || true
	@echo "Built $(TARGET).gba successfully!"

clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).gba
