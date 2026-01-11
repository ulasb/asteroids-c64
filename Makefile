# Makefile for Asteroids C64

# Compiler tools
CC = cl65
AS = ca65
LD = ld65

# Flags
CFLAGS = -t c64 -O
ASFLAGS = -t c64
LDFLAGS = -C c64-simple.cfg

# Target
TARGET = asteroids

# Default target (C version)
all: $(TARGET).prg

# C version
c: $(TARGET).prg

$(TARGET).prg: asteroids.c asteroids.h
	$(CC) $(CFLAGS) -o $(TARGET).prg asteroids.c

# Assembly version
asm: $(TARGET)_asm.prg

$(TARGET)_asm.prg: asteroids.s c64-simple.cfg
	$(AS) $(ASFLAGS) asteroids.s -o asteroids.o
	$(LD) $(LDFLAGS) -o $(TARGET)_asm.prg asteroids.o

# Clean
clean:
	rm -f *.o *.prg

# Run C version
run: $(TARGET).prg
	x64 $(TARGET).prg

# Run assembly version  
run-asm: $(TARGET)_asm.prg
	x64 $(TARGET)_asm.prg

.PHONY: all c asm clean run run-asm
