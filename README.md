# Asteroids C64

A port of the Python Asteroids game for the Commodore 64. Implemented in both C (using cc65) and 6502 Assembly. Based on [asteroids-python](https://github.com/ulasb/asteroids-python).

## Status

**⚠️ CURRENT STATE: NEITHER VERSION IS SATISFACTORY**

- **C Version**: Partially functional but **slow and unplayable** - basic gameplay exists but performance is terrible
- **Assembly Version**: Broken demo - only shows player movement, no actual game

## Overview

⚠️ **This project is currently BROKEN and NOT PLAYABLE.**

This was an attempt to port the Python Asteroids game to the Commodore 64, but neither the C nor assembly implementations are satisfactory. The C version exists but is too slow to be fun, while the assembly version is just a minimal movement demo.

The project demonstrates the technical challenges of porting modern games to 1980s hardware, but does not deliver an enjoyable gaming experience.

## Intended Features (NOT IMPLEMENTED PROPERLY)

The following features were attempted but are either broken, incomplete, or unplayably slow:

- **Character-based graphics**: Basic implementation exists
- **Player controls**: Rotate, thrust, and fire mechanics (slow/unresponsive in C version)
- **Asteroid splitting**: Exists in C version but too slow to be fun
- **Screen wrapping**: Objects wrap around screen edges
- **Level progression**: Basic implementation in C version
- **Scoring system**: Points awarded for destroying asteroids (C version)
- **Lives system**: Start with 3 lives (C version)

## Controls

### Joystick (Port 2)
- **Up**: Thrust forward
- **Left/Right**: Rotate ship
- **Fire button**: Shoot

### Keyboard Alternative
- **W**: Thrust
- **A**: Rotate left
- **D**: Rotate right
- **Space**: Fire
- **R**: Restart (after Game Over)

## Building

### Prerequisites

- **cc65**: Cross-compiler suite for 6502-based systems
  - Download from: https://github.com/cc65/cc65
  - Or install via package manager:
    - macOS: `brew install cc65`
    - Linux: `sudo apt-get install cc65` (Debian/Ubuntu)
    - Windows: Download from GitHub releases

### Compile C Version

```bash
make
# or
make c
```

This will create `asteroids.prg` which can be run on a C64 or emulator.


### Clean

```bash
make clean
```

## Running

### Using an Emulator (Recommended)

**VICE** (cross-platform): https://vice-emu.sourceforge.io/

1. Build the C version: `make`
2. Run in VICE: `make run` or `x64 asteroids.prg`

⚠️ **WARNING**: The C version exists but is too slow to be enjoyable - it's a technical demo, not a playable game.

Other emulators:
- **CCS64** (Windows)
- **Hoxs64** (Windows)
- **Micro64** (online)

### On Real Hardware

1. Build the game: `make` or `make asm`
2. Transfer the `.prg` file to your C64 (via disk, SD2IEC, etc.)
3. Load and run:
   ```
   LOAD "ASTEROIDS",8,1
   RUN
   ```

## Implementation Details

### C Version
- **Language**: C (compiled with cc65)
- **Status**: Technically functional but unplayably slow
- **Performance**: Terrible (C compiler overhead makes it unresponsive)
- **Features**: Has all game mechanics but too slow to be enjoyable

### Assembly Version
- **Language**: 6502 Assembly
- **Status**: Deleted - was just a broken movement demo
- **Performance**: N/A (never completed)
- **Features**: Only player movement, no actual game

## Technical Details

- **Graphics Mode**: Character graphics (40x25)
- **Memory**: Designed to fit in standard C64 memory
- **Input**: Joystick port 2 ($DC00) or keyboard
- **Trigonometry**: Pre-calculated lookup tables (24 values for 15° steps)
- **Build System**: Makefile

## Project Structure

```
asteroids-c64/
├── asteroids.c      # ⚠️ C version - EXISTS but SLOW/UNPLAYABLE
├── asteroids.h      # Header file with constants and structures
├── c64-simple.cfg   # Linker config (unused)
├── Makefile         # Build configuration
├── README.md        # This file (with honest assessment)
├── DESIGN.md        # Design documentation
└── LICENSE          # License file
```

**Note**: Assembly version (`asteroids.s`) was deleted as it was a broken, incomplete demo that didn't work.

## Limitations

- Character graphics are less visually appealing than vector graphics
- C version performance is limited by compiler overhead
- Assembly version is minimal (player movement demo only)
- No sound effects (SID chip support can be added)
- Maximum 20 asteroids on screen (C version)
- Maximum 10 bullets on screen (C version)

## Future Work

- **Complete Assembly Version**: Full game implementation in assembly
- **Sprite Graphics**: Use hardware sprites for better visuals
- **Sound Effects**: Implement SID chip sound for shooting/explosions
- **High Score Table**: Save to disk using file I/O
- **Enemy Saucers**: Add hostile ships that shoot back

## Credits

- Original Python implementation: [asteroids-python](https://github.com/ulasb/asteroids-python)
- C64 C implementation: Attempted port (technically works but too slow)
- C64 Assembly implementation: Abandoned due to complexity

## Lessons Learned

This project demonstrates the extreme challenges of porting modern games to 1980s hardware:
- C64's 1MHz 6502 processor is severely underpowered for real-time games
- Character graphics limit visual appeal
- cc65 C compiler adds significant overhead
- Assembly programming for C64 is extremely time-consuming
- The original Python game relies on modern computing power that the C64 simply cannot match

The project serves as a technical demonstration rather than a playable game.

## License

Mozilla Public License Version 2.0 (see LICENSE file)
