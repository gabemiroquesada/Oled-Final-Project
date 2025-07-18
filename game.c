#include "I2C.h"
#include "OLED.h"
#include "game.h"
#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include <stdio.h>
#include "pongcourt.h"

#define PADDLE_HEIGHT 12
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define CPU_X 12
#define PLAYER_X 115
#define BALL_RATE 16
#define PADDLE_RATE 33
#define UP_BUTTON (1 << 2)   // PD2
#define DOWN_BUTTON (1 << 3) // PD3

uint8_t cpu_y = 8, player_y = 8;
uint8_t player_score = 0, cpu_score = 0;

void drawScore() {
    char score[10];
    sprintf(score, "P:%d C:%d", player_score, cpu_score);
    OLED_YX(1, 4); 
    OLED_Write_String(score);
}

void handleScoring(bool playerScored) {
    OLED_DrawPixel(ball_x, ball_y, 0); 
    OLED_Update();

    OLED_Clear(); 
    drawCourt();  
    drawScore();  

    OLED_YX(2, 3); 
    if (playerScored) {
        OLED_Write_String("Player Scores!");
				Delay_ms(4000);
				OLED_Clear();
    } else {
        OLED_Write_String("CPU Scores!");
				Delay_ms(4000);
				OLED_Clear();
    }

    OLED_Clear(); 
    drawCourt();  
    drawScore(); 

    ball_x = SCREEN_WIDTH / 2;
    ball_y = SCREEN_HEIGHT / 2;
    ball_dir_x = playerScored ? -1 : 1; 
    ball_dir_y = (ball_dir_y == 0) ? 1 : -ball_dir_y;

    OLED_Update();

    ball_update = millis() + 500; 
}

void Pong_Loop(void) {
    OLED_Clear();
    drawCourt();

    while (1) {
        unsigned long current_time = millis();
        bool update = false;

        // Ball movement
        if (current_time > ball_update) {
            uint8_t new_x = ball_x + ball_dir_x;
            uint8_t new_y = ball_y + ball_dir_y;

            // Wall collisions
            if (new_y == 0 || new_y == SCREEN_HEIGHT - 1) {
                ball_dir_y = -ball_dir_y; // Reverse 
            }

            // Paddle collisions
            if (new_x == CPU_X && new_y >= cpu_y && new_y < cpu_y + PADDLE_HEIGHT) {
                ball_dir_x = -ball_dir_x; 
            } else if (new_x == PLAYER_X && new_y >= player_y && new_y < player_y + PADDLE_HEIGHT) {
                ball_dir_x = -ball_dir_x;
            }

            // Scoring logic
            if (new_x < CPU_X) { // Player scores
                player_score++;
                if (player_score == 3) { // Check for game over
                    handleGameOver(true);
                    return; 
                }
                handleScoring(true);
                continue; 
            } else if (new_x > PLAYER_X) { // CPU scores
                cpu_score++;
                if (cpu_score == 3) { // Check for game over
                    handleGameOver(false);
                    return; 
                }
                handleScoring(false);
                continue; 
            }

            // Update ball position
            OLED_DrawPixel(ball_x, ball_y, 0); // Erase the previous ball
            ball_x = new_x;
            ball_y = new_y;
            OLED_DrawPixel(ball_x, ball_y, 1); // Draw the new ball

            update = true;
            ball_update = current_time + BALL_RATE;
        }

        // Paddle movement
        if (current_time > paddle_update) {
            // CPU paddle
            OLED_DrawRect(CPU_X, cpu_y, 1, PADDLE_HEIGHT, 0); // Erase previous paddle
            if (cpu_y + (PADDLE_HEIGHT / 2) > ball_y) {
                if (cpu_y > 0) cpu_y--; // Move up
            } else if (cpu_y + (PADDLE_HEIGHT / 2) < ball_y) {
                if (cpu_y + PADDLE_HEIGHT < SCREEN_HEIGHT) cpu_y++; // Move down
            }
            OLED_DrawRect(CPU_X, cpu_y, 1, PADDLE_HEIGHT, 1); // Draw updated paddle

            // Player paddle
            OLED_DrawRect(PLAYER_X, player_y, 1, PADDLE_HEIGHT, 0); 
            if (!(GPIOD->DATA & UP_BUTTON) && player_y > 0) {
                player_y--;
            }
            if (!(GPIOD->DATA & DOWN_BUTTON) && player_y + PADDLE_HEIGHT < SCREEN_HEIGHT) {
                player_y++;
            }
            OLED_DrawRect(PLAYER_X, player_y, 1, PADDLE_HEIGHT, 1); 

            update = true;
            paddle_update = current_time + PADDLE_RATE;
        }

        if (update) {
            OLED_Update(); 
        }
    }
}

void handleGameOver(bool playerWins) {
    OLED_Clear(); 

    // Display the final score
    OLED_YX(1, 4);
    char finalScore[16];
    sprintf(finalScore, "P:%d C:%d", player_score, cpu_score);
    OLED_Write_String(finalScore);

    // Display the winner
    OLED_YX(2, 3);
    if (playerWins) {
        OLED_Write_String("Player Wins!");
				Delay_ms(5000);
    } else {
        OLED_Write_String("CPU Wins!");
				Delay_ms(5000);
    }
		player_score = 0;
		cpu_score = 0;

}
