#include "pongcourt.h"
#include "OLED.h"

uint8_t ball_x = 64, ball_y = 16;
int8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update = 0, paddle_update = 0;

void drawCourt(void) {
    OLED_DrawRect(0, 0, 128, 32, 1); 
}
