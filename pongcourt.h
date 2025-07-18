#ifndef PONGCOURT_H
#define PONGCOURT_H

#include <stdint.h>

extern uint8_t ball_x, ball_y;
extern int8_t ball_dir_x, ball_dir_y;
extern unsigned long ball_update, paddle_update;

void drawCourt(void);

#endif 
