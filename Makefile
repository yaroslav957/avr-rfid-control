MCU = atmega32
F_CPU = 8000000UL
PROJECT = main

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size

SRCDIR = src
LIBDIR = lib
BUILD_DIR = build

CFLAGS = -std=c17
CFLAGS += -mmcu=$(MCU)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -Os
CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wpointer-arith
CFLAGS += -ffunction-sections -fdata-sections
# опасная штука, которая трахнет
# за любой пропущенный volatile
# CFLAGS += -flto

CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(LIBDIR)
CFLAGS += -I$(LIBDIR)/blake2s
CFLAGS += -MMD -MP

LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map
LDFLAGS += -Wl,--gc-sections
# LDFLAGS += -flto

SRCS = $(wildcard $(SRCDIR)/*.c) \
       $(wildcard $(LIBDIR)/*/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean size hexelf

all: hexelf size
hexelf: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex

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

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
