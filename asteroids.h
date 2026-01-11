/*
 * Asteroids C64 - Header file
 */

#ifndef ASTEROIDS_H
#define ASTEROIDS_H

/* Screen dimensions (character grid) */
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 25

/* Game constants */
#define MAX_ASTEROIDS 20
#define MAX_BULLETS 10

/* Asteroid structure (using character coordinates) */
typedef struct {
    int x, y;  /* Character grid coordinates (0-39, 0-24) */
    int vx, vy; /* Velocity in character units */
    int size; /* 3=large, 2=medium, 1=small */
    unsigned char active;
} Asteroid;

/* Bullet structure */
typedef struct {
    int x, y;  /* Character grid coordinates */
    int vx, vy; /* Velocity in character units */
    unsigned char active;
} Bullet;

#endif /* ASTEROIDS_H */
