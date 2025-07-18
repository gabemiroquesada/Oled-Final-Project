/*
#include "TM4C123GH6PM.h"
#include "I2C.h"
#include "OLED.h"
#include "game.h"
#include "galaga.h"
#include "pong2player.h"
#include "pongcourt.h"

#define UP_BUTTON (1 << 2)   // PD2
#define DOWN_BUTTON (1 << 3) // PD3

void GPIO_Init(void);
void initSysTick(void);

int main() {
    GPIO_Init();   
    initSysTick(); 
    I2C3_Init();   
    OLED_Init();   
    OLED_Clear(); 
		//Test_Screen();
    Pong_Loop();   
    return 0;
}

void GPIO_Init(void) {
    SYSCTL->RCGCGPIO |= 0x08;    // Enable clock for Port D
    GPIOD->DIR &= ~(UP_BUTTON | DOWN_BUTTON); // PD2 and PD3 input
    GPIOD->DEN |= (UP_BUTTON | DOWN_BUTTON);  // Enable dig function
    GPIOD->PUR |= (UP_BUTTON | DOWN_BUTTON);  // Enable pull-up resistors
}

*/
#include "TM4C123GH6PM.h"
#include "I2C.h"
#include "OLED.h"
#include "pong2player.h"

void GPIO_Init(void);
void initSysTick(void);

int main() {
    GPIO_Init();      
    initSysTick();   
    I2C3_Init();      
    OLED_Init();     
    OLED_Clear();     
    Pong_2P_Loop();  
    return 0;
}

void GPIO_Init(void) {
    // Enable clock for Port D and Port A
    SYSCTL->RCGCGPIO |= 0x09; 
    while (!(SYSCTL->PRGPIO & 0x09)); 

    // PD2 and PD3 for Player 1 buttons
    GPIOD->DIR &= ~((1 << 2) | (1 << 3)); // Set PD2 and PD3 as inputs
    GPIOD->DEN |= ((1 << 2) | (1 << 3));  // Enable digital function
    GPIOD->PUR |= ((1 << 2) | (1 << 3));  // Enable pull-up resistors 

    // PA4 and PA5 for Player 2 buttons
    GPIOA->DIR &= ~((1 << 4) | (1 << 5)); // Set PA4 and PA5 as inputs
    GPIOA->DEN |= ((1 << 4) | (1 << 5));  // Enable digital function 
    GPIOA->PUR |= ((1 << 4) | (1 << 5));  // Enable pull-up resistors 
}


