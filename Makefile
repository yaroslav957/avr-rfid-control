MCU = atmega32
MCU_DEF = __AVR_ATmega32__
F_CPU = 8000000UL

PROJECT = main

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size

SRCDIR = src
LIBDIR = lib
BUILD_DIR = build

rwildcard = $(foreach d,$(wildcard $(1:=/*)), \
		$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = $(call rwildcard,$(SRCDIR),*.c) \
		$(call rwildcard,$(LIBDIR),*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

INCLUDES = -I$(SRCDIR)
INCLUDES += -I$(LIBDIR)
INCLUDES += -isystem $(LIBDIR)/blake2s
INCLUDES += -isystem $(LIBDIR)/glcd

CFLAGS = -std=c17
CFLAGS += -mmcu=$(MCU)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -Os
CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wpointer-arith
CFLAGS += -ffunction-sections -fdata-sections
# CFLAGS += -flto

CFLAGS += $(INCLUDES)
CFLAGS += -MMD -MP

LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map
LDFLAGS += -Wl,--gc-sections
# LDFLAGS += -flto

.PHONY: all clean size flags build

all: build
build: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex

$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@mkdir -p $(dir $@)
	@echo "  LD   $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "  HEX  $@"
	@$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  CC   $<"
	@$(CC) $(CFLAGS) -c $< -o $@

size: $(BUILD_DIR)/$(PROJECT).elf
	@$(SIZE) -C --mcu=$(MCU) $<

flags: compile_flags.txt
compile_flags.txt: Makefile
	@printf '%s\n' \
		-target avr \
		-mmcu=$(MCU) \
		-isystem /usr/avr/include \
		-D$(MCU_DEF)\
		-DF_CPU=$(F_CPU) \
		-Os \
		$(INCLUDES) \
		-std=c17 \
		-Wall -Wextra > $@
	@echo "Generated $@"
	
clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
