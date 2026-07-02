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

INCLUDES  = -I$(SRCDIR)
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

SRCS = $(wildcard $(SRCDIR)/*.c) \
       $(wildcard $(LIBDIR)/*/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean size flags build

all: build size
build: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex

$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

size: $(BUILD_DIR)/$(PROJECT).elf
	@echo ""
	@echo "~~~ Static size analysis ~~~"
	$(SIZE) -C --mcu=$(MCU) $<

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
