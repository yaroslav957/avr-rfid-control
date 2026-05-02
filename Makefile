MCU = atmega2560
F_CPU = 8000000UL
CC = avr-gcc

SRCDIR = src
CFLAGS = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) -Wall -I$(SRCDIR)

TARGET = main.elf
SRCS = $(wildcard $(SRCDIR)/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: all
	qemu-system-avr -machine mega2560 -bios $(TARGET) -nographic 

clean:
	rm -f $(TARGET)
