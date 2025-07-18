#include "I2C.h"
#include "OLED.h"
#include "Pong2player.h"
#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include <stdio.h>
#include "pongcourt.h"

#define PADDLE_HEIGHT 12
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define PLAYER1_X 12
#define PLAYER2_X 115
#define BALL_RATE 16
#define PADDLE_RATE 33
#define PLAYER1_UP_BUTTON (1 << 2)   // PD2
#define PLAYER1_DOWN_BUTTON (1 << 3) // PD3
#define PLAYER2_UP_BUTTON (1 << 4)   // PA4
#define PLAYER2_DOWN_BUTTON (1 << 5) // PA5

uint8_t player1_y = 8, player2_y = 8;
uint8_t player1_score = 0, player2_score = 0;

void drawScore2p() {
    char score[16];
    sprintf(score, "P1:%d P2:%d", player1_score, player2_score);
    OLED_YX(1, 4); 
    OLED_Write_String(score);
}

void handleScoring2p(bool player1Scored) {
    OLED_DrawPixel(ball_x, ball_y, 0); 
    OLED_Update();

    OLED_Clear(); 
    drawCourt();  
    drawScore2p();  

    OLED_YX(2, 3); 
    if (player1Scored) {
        OLED_Write_String("P1 Scores!");
				Delay_ms(4000);
				OLED_Clear();
    } else {
        OLED_Write_String("P2 Scores!");
				Delay_ms(4000);
				OLED_Clear();
    }

    OLED_Clear(); 
    drawCourt();  
    drawScore2p(); 

    ball_x = SCREEN_WIDTH / 2;
    ball_y = SCREEN_HEIGHT / 2;
    ball_dir_x = player1Scored ? -1 : 1;
    ball_dir_y = (ball_dir_y == 0) ? 1 : -ball_dir_y;

    OLED_Update();

    ball_update = millis() + 500; 
}

void Pong_2P_Loop(void) {
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
                ball_dir_y = -ball_dir_y; 
            }

            // Paddle collisions
            if (new_x == PLAYER1_X && new_y >= player1_y && new_y < player1_y + PADDLE_HEIGHT) {
                ball_dir_x = -ball_dir_x;
            } else if (new_x == PLAYER2_X && new_y >= player2_y && new_y < player2_y + PADDLE_HEIGHT) {
                ball_dir_x = -ball_dir_x;
            }

            // Scoring logic
            if (new_x < PLAYER1_X) { // Player 2 scores
                player2_score++;
                if (player2_score == 3) { // Check for game over
                    handleGameOver2p(false);
                    return; 
                }
                handleScoring2p(false);
                continue; 
            } else if (new_x > PLAYER2_X) { // Player 1 scores
                player1_score++;
                if (player1_score == 3) { // Check for game over
                    handleGameOver2p(true);
                    return; 
                }
                handleScoring2p(true);
                continue; 
            }

            // Update ball position
            OLED_DrawPixel(ball_x, ball_y, 0); 
            ball_x = new_x;
            ball_y = new_y;
            OLED_DrawPixel(ball_x, ball_y, 1); 

            update = true;
            ball_update = current_time + BALL_RATE;
        }

        // Paddle movement
        if (current_time > paddle_update) {
            // Player 1 paddle
            OLED_DrawRect(PLAYER1_X, player1_y, 1, PADDLE_HEIGHT, 0); // Erase previous paddle
            if (!(GPIOD->DATA & PLAYER1_UP_BUTTON) && player1_y > 0) {
                player1_y--;
            }
            if (!(GPIOD->DATA & PLAYER1_DOWN_BUTTON) && player1_y + PADDLE_HEIGHT < SCREEN_HEIGHT) {
                player1_y++;
            }
            OLED_DrawRect(PLAYER1_X, player1_y, 1, PADDLE_HEIGHT, 1); // Draw updated paddle

            // Player 2 paddle
            OLED_DrawRect(PLAYER2_X, player2_y, 1, PADDLE_HEIGHT, 0); 
            if (!(GPIOA->DATA & PLAYER2_UP_BUTTON) && player2_y > 0) {
                player2_y--;
            }
            if (!(GPIOA->DATA & PLAYER2_DOWN_BUTTON) && player2_y + PADDLE_HEIGHT < SCREEN_HEIGHT) {
                player2_y++;
            }
            OLED_DrawRect(PLAYER2_X, player2_y, 1, PADDLE_HEIGHT, 1);

            update = true;
            paddle_update = current_time + PADDLE_RATE;
        }

        if (update) {
            OLED_Update(); 
        }
    }
}

void handleGameOver2p(bool player1Wins) {
    OLED_Clear(); 

    // Display the final score
    OLED_YX(1, 4);
    char finalScore[16];
    sprintf(finalScore, "P1:%d P2:%d", player1_score, player2_score);
    OLED_Write_String(finalScore);

    // Display the winner
    OLED_YX(2, 3);
    if (player1Wins) {
        OLED_Write_String("Player1 Wins!");
				Delay_ms(5000);
    } else {
        OLED_Write_String("Player2 Wins!");
				Delay_ms(5000);
    }
    player1_score = 0;
    player2_score = 0;
}
