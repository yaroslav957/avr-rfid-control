MCU = atmega32
F_CPU = 8000000UL
CC = avr-gcc

SRCDIR = src
LIBDIR = lib
TARGET = main.elf

CFLAGS = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) -Wall
CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(LIBDIR)

SRCS = $(wildcard $(SRCDIR)/*.c) \
       $(wildcard $(LIBDIR)/*.c) \
       $(wildcard $(LIBDIR)/**/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
