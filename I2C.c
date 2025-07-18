#include "TM4C123GH6PM.h"
#include "I2C.h"

void I2C3_Init(void) {
    SYSCTL->RCGCI2C |= 0x08; // Enable clock for I2C3
    SYSCTL->RCGCGPIO |= 0x08; // Enable clock for Port D
    GPIOD->AFSEL |= 0x03;     // Enable alternate functions on PD0, PD1
    GPIOD->PCTL |= 0x00000033;
    GPIOD->ODR |= 0x02;       // Open-drain for SDA (PD1)
    GPIOD->DEN |= 0x03;       // Digital enable
    I2C3->MCR = 0x10;         // Master mode
    I2C3->MTPR = 0x07;        // Set clock
}

void I2C3_Write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data) {
    I2C3->MSA = (slaveAddr << 1) & 0xFE; 
    I2C3->MDR = regAddr;
    I2C3->MCS = 0x03; 
    while (I2C3->MCS & 0x01);
    I2C3->MDR = data;
    I2C3->MCS = 0x05; 
    while (I2C3->MCS & 0x01);
}

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