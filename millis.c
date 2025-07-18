#include "TM4C123GH6PM.h"

volatile unsigned long msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;
}

void initSysTick(void) {
    SysTick->LOAD = 16000 - 1; // 1ms interval at 16 MHz clock
    SysTick->VAL = 0;          // Clear current value
    SysTick->CTRL = 0x07;      // Enable SysTick, enable interrupt, use system clock
}

unsigned long millis(void) {
    return msTicks;
}
