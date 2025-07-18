#include "OLED.h"
#include "I2C.h"
#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include <stdlib.h>

#define PLAYER_Y 28
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define ENEMY_COUNT 5
#define ENEMY_SPEED 1
#define PLAYER_SHOT_SPEED 2
#define ENEMY_SHOT_SPEED 1
#define MOVE_LEFT_BUTTON (1 << 4)   // PA4 - Move left
#define MOVE_RIGHT_BUTTON (1 << 5) // PA5 - Move right
#define FIRE_BUTTON (1 << 6)       // PA6 - Fire shot

typedef struct {
    uint8_t x, y;
    bool active;
} Entity;

static Entity player_shot;
static Entity enemies[ENEMY_COUNT];
static Entity enemy_shot;

static uint8_t player_x = SCREEN_WIDTH / 2;

static unsigned long enemy_update = 0;
static unsigned long player_shot_update = 0;
static unsigned long enemy_shot_update = 0;

void Galaga_Init(void) {
    // Enable clock for Port A
    SYSCTL->RCGCGPIO |= (1 << 0);
    while (!(SYSCTL->PRGPIO & (1 << 0)))
        ;

    // Set PA4, PA5, and PA6 as input for Galaga controls
    GPIOA->DIR &= ~(MOVE_LEFT_BUTTON | MOVE_RIGHT_BUTTON | FIRE_BUTTON);
    GPIOA->DEN |= (MOVE_LEFT_BUTTON | MOVE_RIGHT_BUTTON | FIRE_BUTTON);
    GPIOA->PUR |= (MOVE_LEFT_BUTTON | MOVE_RIGHT_BUTTON | FIRE_BUTTON); // Enable pull-up resistors

    // Initialize enemies
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemies[i].x = rand() % SCREEN_WIDTH;
        enemies[i].y = (i * 5) + 2;
        enemies[i].active = true;
    }
    player_shot.active = false;
    enemy_shot.active = false;

    OLED_Clear();
    OLED_Update();
}

void Galaga_DrawPlayer(void) {
    OLED_DrawRect(player_x, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT, 1); // Draw player
}

void Galaga_DrawEnemies(void) {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (enemies[i].active) {
            OLED_DrawRect(enemies[i].x, enemies[i].y, 4, 4, 1); // Draw active enemies
        }
    }
}

void Galaga_DrawShot(Entity *shot) {
    if (shot->active) {
        OLED_DrawRect(shot->x, shot->y, 2, 2, 1); // Draw shot as a small square
    }
}

void Galaga_MoveEnemies(void) {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (enemies[i].active) {
            enemies[i].y += ENEMY_SPEED;
            if (enemies[i].y >= SCREEN_HEIGHT) {
                enemies[i].y = 0; // Reset enemy to the top
                enemies[i].x = rand() % SCREEN_WIDTH; // Randomize x position
            }
        }
    }
}

void Galaga_MoveShot(Entity *shot, int speed) {
    if (shot->active) {
        shot->y -= speed;
        if (shot->y == 0) {
            shot->active = false; // Deactivate if out of bounds
        }
    }
}

void Galaga_CheckCollisions(void) {
    // Check for player shot hitting an enemy
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (player_shot.active && enemies[i].active) {
            if (player_shot.x >= enemies[i].x && player_shot.x <= enemies[i].x + 4 &&
                player_shot.y >= enemies[i].y && player_shot.y <= enemies[i].y + 4) {
                enemies[i].active = false;
                player_shot.active = false;
            }
        }
    }

    // Check for enemy shot hitting the player
    if (enemy_shot.active) {
        if (enemy_shot.x >= player_x && enemy_shot.x <= player_x + PLAYER_WIDTH &&
            enemy_shot.y >= PLAYER_Y && enemy_shot.y <= PLAYER_Y + PLAYER_HEIGHT) {
            // Game over logic
            OLED_Clear();
            OLED_YX(1, 2);
            OLED_Write_String("Game Over");
            OLED_Update();
            while (1); // Halt the game
        }
    }
}

void Galaga_Loop(void) {
    Galaga_Init();

    while (1) {
        unsigned long current_time = millis();

        // Handle player movement
        if (!(GPIOA->DATA & MOVE_LEFT_BUTTON) && player_x > 0) {
            player_x--; // Move left
        }
        if (!(GPIOA->DATA & MOVE_RIGHT_BUTTON) && player_x + PLAYER_WIDTH < SCREEN_WIDTH) {
            player_x++; // Move right
        }

        // Handle player shooting
        if (player_shot.active == false && !(GPIOA->DATA & FIRE_BUTTON)) {
            player_shot.x = player_x + (PLAYER_WIDTH / 2); // Center shot horizontally
            player_shot.y = PLAYER_Y - 1; // Shot starts just above the player
            player_shot.active = true;
        }

        // Enemy movement update
        if (current_time > enemy_update) {
            Galaga_MoveEnemies();
            enemy_update = current_time + 200;
        }

        // Player shot update
        if (current_time > player_shot_update) {
            Galaga_MoveShot(&player_shot, PLAYER_SHOT_SPEED);
            player_shot_update = current_time + 50;
        }

        // Enemy shot update
        if (current_time > enemy_shot_update) {
            if (!enemy_shot.active) {
                // Randomly fire from an active enemy
                for (int i = 0; i < ENEMY_COUNT; i++) {
                    if (enemies[i].active) {
                        enemy_shot.x = enemies[i].x + 2; // Center shot horizontally
                        enemy_shot.y = enemies[i].y + 4; // Shot starts below the enemy
                        enemy_shot.active = true;
                        break;
                    }
                }
            } else {
                Galaga_MoveShot(&enemy_shot, -ENEMY_SHOT_SPEED);
            }
            enemy_shot_update = current_time + 100;
        }

        // Check collisions
        Galaga_CheckCollisions();

        // Redraw the game
        OLED_Clear();
        Galaga_DrawPlayer();
        Galaga_DrawEnemies();
        Galaga_DrawShot(&player_shot);
        Galaga_DrawShot(&enemy_shot);
        OLED_Update();
    }
}

