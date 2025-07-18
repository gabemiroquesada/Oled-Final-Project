#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C3_Init(void);
void I2C3_Write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
void initSysTick(void);
unsigned long millis(void);

#endif 
