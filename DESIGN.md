# Asteroids C64 - Design Document

## Overview
This is a simplified port of the Python Asteroids game for the Commodore 64, implemented in C using the cc65 compiler. Due to hardware constraints, several simplifications have been made while maintaining the core gameplay experience.

## Hardware Constraints
- **Memory**: 64KB RAM (system uses ~40KB, leaving ~24KB for program and data)
- **Processor**: 1MHz 6502
- **Graphics**: Sprites (8x8 pixels, max 8 per scanline) or character graphics
- **Input**: Joystick port 2 or keyboard
- **Sound**: SID chip (3 voices)

## Simplifications from Python Version

### Graphics
- **Python**: Vector wireframe graphics
- **C64**: Character-based graphics
  - Player ship: `*` character
  - Asteroids: `O` (large), `o` (medium), `.` (small)
  - Bullets: `.` character
  - Screen wrap: Maintained via coordinate wrapping

### Physics
- **Python**: Full momentum and friction system
- **C64**: Simplified physics
  - Constant velocity for asteroids (no momentum)
  - Simplified thrust model with friction
  - Screen wrapping maintained
  - Collision detection: Distance-based (simple distance checks)

### Features
- **Maintained**:
  - Player controls (rotate, thrust, fire)
  - Asteroid splitting (large → medium → small)
  - Screen wrapping
  - Scoring system
  - Level progression (increasing asteroid count/speed)
  
- **Simplified/Removed**:
  - Enemy saucers (too complex for initial version)
  - High score persistence (requires disk I/O)
  - Sound effects (can be added later)
  - Complex collision detection (using simple distance checks)

## Implementation Approach

### Language & Compiler
- **C** compiled with **cc65**
- Provides better performance and easier development than BASIC
- Access to C64 hardware via cc65 libraries (conio.h, peekpoke.h)

### Graphics
1. **Character Graphics Mode**: Uses standard C64 character graphics for simplicity
2. **Screen Updates**: Direct screen memory access via conio functions
3. **Future Enhancement**: Could use sprites for player/bullets if needed

### Data Structures
- Arrays for asteroids and bullets
- Static game state (player position, score, level, lives)
- Pre-calculated trigonometry lookup tables

### Game Loop
1. Handle input (joystick/keyboard)
2. Update physics (player, asteroids, bullets)
3. Check collisions (bullet-asteroid, player-asteroid)
4. Draw screen
5. Check level completion

### Input Handling
- Joystick port 2: Direct memory read from $DC00
- Keyboard: Via conio kbhit()/cgetc()
- Debouncing for fire button

## Code Structure

```
asteroids.c
├── Game state variables (static)
├── Player state (position, angle, velocity)
├── Asteroid array
├── Bullet array
├── Trigonometry lookup tables
├── Helper functions (trig index, wrap, distance)
├── init_level() - Initialize new level
├── handle_input() - Process joystick/keyboard
├── update_physics() - Update all game objects
├── check_collisions() - Detect hits
├── split_asteroid() - Create smaller asteroids
├── draw_screen() - Render game state
├── check_level_complete() - Advance level
├── game_over_screen() - Display game over
└── main() - Game loop

asteroids.h
├── Screen dimensions
├── Game constants (MAX_ASTEROIDS, MAX_BULLETS)
├── Asteroid structure
└── Bullet structure
```

## Performance Considerations

- Fixed-point math could improve performance (currently using floats)
- Trigonometry lookup tables (24 values) avoid runtime calculations
- Simple collision detection (distance-squared checks)
- Character graphics for fast rendering
- Static allocations to avoid heap fragmentation

## Future Enhancements

1. **Sprite Graphics**: Use hardware sprites for better visuals
2. **Sound Effects**: Implement SID chip sound for shooting/explosions
3. **High Score Table**: Save to disk using cc65 file I/O
4. **Enemy Saucers**: Add hostile ships that shoot back
5. **Particle Effects**: Visual feedback for explosions
6. **Optimization**: Inline assembly for critical sections
7. **Fixed-Point Math**: Replace floats with fixed-point for speed

## Build System

- **Makefile**: Simple build configuration using cl65 (cc65 compiler/linker)
- Single compilation step: `cl65 -t c64 -O -o asteroids.prg asteroids.c`
- Output: `.prg` file ready for C64 or emulator
