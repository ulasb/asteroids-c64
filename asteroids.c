/*
 * Asteroids C64 - C Implementation using cc65
 * Based on asteroids-python
 * High-performance version with direct screen memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <peekpoke.h>

#include "asteroids.h"

/* Game state */
static int score = 0;
static int level = 1;
static int lives = 3;
static int gameover = 0;
static int invulnerable = 0;

/* Player state (using character coordinates directly) */
static int player_x = 20;
static int player_y = 13;
static int player_angle = 0;
static int player_vx = 0;
static int player_vy = 0;

/* Asteroids */
static Asteroid asteroids[MAX_ASTEROIDS];
static int asteroid_count = 0;

/* Bullets */
static Bullet bullets[MAX_BULLETS];
static int bullet_count = 0;

/* Screen memory buffer */
static unsigned char screen_buffer[40 * 23];

/* Sine/Cosine lookup tables */
static const signed char cos_table[24] = {
    1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1,
    -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1
};

static const signed char sin_table[24] = {
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1,
    0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 1, 1
};

/* Helper functions */
static int get_trig_index(int angle) {
    int idx = angle / 15;
    while (idx < 0) idx += 24;
    while (idx >= 24) idx -= 24;
    return idx;
}

/* Initialize a new level */
static void init_level(void) {
    int i;
    int dist_to_player;
    int ax;
    int ay;
    int attempts;
    
    asteroid_count = 5 + level * 2;
    if (asteroid_count > MAX_ASTEROIDS) asteroid_count = MAX_ASTEROIDS;
    
    for (i = 0; i < asteroid_count; ++i) {
        attempts = 0;
        do {
            ax = rand() % 40;
            ay = 2 + (rand() % 23);
            dist_to_player = (ax - player_x) * (ax - player_x) + 
                           (ay - player_y) * (ay - player_y);
            attempts++;
        } while (dist_to_player < 25 && attempts < 100);
        
        asteroids[i].x = ax;
        asteroids[i].y = ay;
        asteroids[i].vx = (rand() % 3) - 1;
        asteroids[i].vy = (rand() % 3) - 1;
        asteroids[i].size = 3;
        asteroids[i].active = 1;
    }
}

/* Handle input */
static void handle_input(void) {
    unsigned char joy;
    int idx;
    int i;
    char c;
    static unsigned char last_fire = 1;
    
    joy = PEEK(0xDC00);
    
    if (!(joy & 0x04)) {
        player_angle -= 15;
        if (player_angle < 0) player_angle += 360;
    }
    
    if (!(joy & 0x08)) {
        player_angle += 15;
        if (player_angle >= 360) player_angle -= 360;
    }
    
    if (!(joy & 0x01)) {
        idx = get_trig_index(player_angle);
        player_vx += cos_table[idx];
        player_vy += sin_table[idx];
        if (player_vx > 2) player_vx = 2;
        if (player_vx < -2) player_vx = -2;
        if (player_vy > 2) player_vy = 2;
        if (player_vy < -2) player_vy = -2;
    }
    
    if (!(joy & 0x10) && last_fire) {
        if (bullet_count < MAX_BULLETS) {
            for (i = 0; i < MAX_BULLETS; ++i) {
                if (!bullets[i].active) {
                    bullets[i].active = 1;
                    bullets[i].x = player_x;
                    bullets[i].y = player_y;
                    idx = get_trig_index(player_angle);
                    bullets[i].vx = cos_table[idx] * 2;
                    bullets[i].vy = sin_table[idx] * 2;
                    bullet_count++;
                    break;
                }
            }
        }
    }
    last_fire = (joy & 0x10);
    
    if (kbhit()) {
        c = cgetc();
        if (c == 'a' || c == 'A') {
            player_angle -= 15;
            if (player_angle < 0) player_angle += 360;
        }
        if (c == 'd' || c == 'D') {
            player_angle += 15;
            if (player_angle >= 360) player_angle -= 360;
        }
        if (c == 'w' || c == 'W') {
            idx = get_trig_index(player_angle);
            player_vx += cos_table[idx];
            player_vy += sin_table[idx];
            if (player_vx > 2) player_vx = 2;
            if (player_vx < -2) player_vx = -2;
            if (player_vy > 2) player_vy = 2;
            if (player_vy < -2) player_vy = -2;
        }
        if (c == ' ') {
            if (bullet_count < MAX_BULLETS) {
                for (i = 0; i < MAX_BULLETS; ++i) {
                    if (!bullets[i].active) {
                        bullets[i].active = 1;
                        bullets[i].x = player_x;
                        bullets[i].y = player_y;
                        idx = get_trig_index(player_angle);
                        bullets[i].vx = cos_table[idx] * 2;
                        bullets[i].vy = sin_table[idx] * 2;
                        bullet_count++;
                        break;
                    }
                }
            }
        }
    }
}

/* Update physics */
static void update_physics(void) {
    int i;
    
    if (player_vx > 0) player_vx--;
    if (player_vx < 0) player_vx++;
    if (player_vy > 0) player_vy--;
    if (player_vy < 0) player_vy++;
    
    player_x += player_vx;
    player_y += player_vy;
    
    if (player_x < 0) player_x = 39;
    if (player_x >= 40) player_x = 0;
    if (player_y < 2) player_y = 24;
    if (player_y > 24) player_y = 2;
    
    for (i = 0; i < asteroid_count; ++i) {
        if (asteroids[i].active) {
            asteroids[i].x += asteroids[i].vx;
            asteroids[i].y += asteroids[i].vy;
            if (asteroids[i].x < 0) asteroids[i].x = 39;
            if (asteroids[i].x >= 40) asteroids[i].x = 0;
            if (asteroids[i].y < 2) asteroids[i].y = 24;
            if (asteroids[i].y > 24) asteroids[i].y = 2;
        }
    }
    
    for (i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].active) {
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            if (bullets[i].x < 0 || bullets[i].x >= 40 ||
                bullets[i].y < 2 || bullets[i].y > 24) {
                bullets[i].active = 0;
                bullet_count--;
            }
        }
    }
}

/* Split asteroid */
static void split_asteroid(int idx) {
    Asteroid *a;
    int free_idx;
    int i;
    
    if (asteroid_count >= MAX_ASTEROIDS) return;
    
    a = &asteroids[idx];
    
    free_idx = -1;
    for (i = 0; i < MAX_ASTEROIDS; ++i) {
        if (!asteroids[i].active) {
            free_idx = i;
            break;
        }
    }
    
    if (free_idx >= 0) {
        asteroids[free_idx].x = a->x;
        asteroids[free_idx].y = a->y;
        asteroids[free_idx].vx = (rand() % 3) - 1;
        asteroids[free_idx].vy = (rand() % 3) - 1;
        asteroids[free_idx].size = a->size;
        asteroids[free_idx].active = 1;
        asteroid_count++;
    }
}

/* Check collisions */
static void check_collisions(void) {
    int b;
    int a;
    int dx;
    int dy;
    int dist2;
    int threshold;
    
    for (b = 0; b < MAX_BULLETS; ++b) {
        if (!bullets[b].active) continue;
        
        for (a = 0; a < asteroid_count; ++a) {
            if (!asteroids[a].active) continue;
            
            dx = bullets[b].x - asteroids[a].x;
            dy = bullets[b].y - asteroids[a].y;
            dist2 = dx * dx + dy * dy;
            threshold = (2 + asteroids[a].size) * (2 + asteroids[a].size);
            
            if (dist2 < threshold) {
                asteroids[a].size--;
                bullets[b].active = 0;
                bullet_count--;
                score += (4 - asteroids[a].size) * 10;
                
                if (asteroids[a].size > 0) {
                    split_asteroid(a);
                } else {
                    asteroids[a].active = 0;
                }
                break;
            }
        }
    }
    
    if (invulnerable <= 0) {
        for (a = 0; a < asteroid_count; ++a) {
            if (!asteroids[a].active) continue;
            
            dx = player_x - asteroids[a].x;
            dy = player_y - asteroids[a].y;
            dist2 = dx * dx + dy * dy;
            threshold = (2 + asteroids[a].size) * (2 + asteroids[a].size);
            
            if (dist2 < threshold) {
                lives--;
                if (lives <= 0) {
                    gameover = 1;
                } else {
                    player_x = 20;
                    player_y = 13;
                    player_vx = 0;
                    player_vy = 0;
                    invulnerable = 30;
                }
                break;
            }
        }
    } else {
        invulnerable--;
    }
}

/* Fast screen drawing using direct memory access */
static void draw_screen(void) {
    int i;
    int offset;
    unsigned char *screen_mem;
    static int status_dirty = 1;
    
    /* Clear screen buffer */
    for (i = 0; i < 40 * 23; ++i) {
        screen_buffer[i] = 32; /* Space */
    }
    
    /* Draw player to buffer */
    if (invulnerable <= 0 || (invulnerable % 4) < 2) {
        if (player_y >= 2 && player_y <= 24 && player_x >= 0 && player_x < 40) {
            offset = (player_y - 2) * 40 + player_x;
            screen_buffer[offset] = '*';
        }
    }
    
    /* Draw asteroids to buffer */
    for (i = 0; i < asteroid_count; ++i) {
        if (!asteroids[i].active) continue;
        if (asteroids[i].y >= 2 && asteroids[i].y <= 24 && 
            asteroids[i].x >= 0 && asteroids[i].x < 40) {
            offset = (asteroids[i].y - 2) * 40 + asteroids[i].x;
            switch (asteroids[i].size) {
                case 3: screen_buffer[offset] = 'O'; break;
                case 2: screen_buffer[offset] = 'o'; break;
                case 1: screen_buffer[offset] = '.'; break;
            }
        }
    }
    
    /* Draw bullets to buffer */
    for (i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].active) continue;
        if (bullets[i].y >= 2 && bullets[i].y <= 24 && 
            bullets[i].x >= 0 && bullets[i].x < 40) {
            offset = (bullets[i].y - 2) * 40 + bullets[i].x;
            screen_buffer[offset] = '.';
        }
    }
    
    /* Copy buffer to screen memory (faster than conio) */
    screen_mem = (unsigned char *)0x0400 + 80; /* Skip first 2 rows */
    for (i = 0; i < 40 * 23; ++i) {
        POKE((unsigned int)screen_mem + i, screen_buffer[i]);
    }
    
    /* Update status bar only when changed */
    if (status_dirty) {
        gotoxy(0, 0);
        cprintf("SCORE:%d LVL:%d LIVES:%d", score, level, lives);
        status_dirty = 0;
    }
}

/* Check if level complete */
static void check_level_complete(void) {
    int count = 0;
    int i;
    
    for (i = 0; i < asteroid_count; ++i) {
        if (asteroids[i].active) count++;
    }
    
    if (count == 0) {
        level++;
        init_level();
    }
}

/* Game over screen */
static void game_over_screen(void) {
    char c;
    
    clrscr();
    cprintf("GAME OVER!\n");
    cprintf("FINAL SCORE: %d\n", score);
    cprintf("LEVEL REACHED: %d\n", level);
    cprintf("PRESS R TO RESTART\n");
    
    while (1) {
        if (kbhit()) {
            c = cgetc();
            if (c == 'r' || c == 'R') {
                break;
            }
        }
    }
}

/* Main game loop */
int main(void) {
    while (1) {
        score = 0;
        level = 1;
        lives = 3;
        gameover = 0;
        player_x = 20;
        player_y = 13;
        player_angle = 0;
        player_vx = 0;
        player_vy = 0;
        bullet_count = 0;
        invulnerable = 30;
        memset(bullets, 0, sizeof(bullets));
        memset(asteroids, 0, sizeof(asteroids));
        
        clrscr();
        init_level();
        
        while (!gameover) {
            handle_input();
            update_physics();
            check_collisions();
            draw_screen();
            check_level_complete();
        }
        
        game_over_screen();
    }
    
    return 0;
}
