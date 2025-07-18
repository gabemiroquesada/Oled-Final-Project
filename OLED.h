#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_Init(void);
void OLED_Update(void);
void OLED_Clear(void);
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void OLED_YX(uint8_t row, uint8_t column); 
void OLED_Write_String(const char *str);  
void OLED_Write_Integer(uint8_t i);       
void OLED_Write_Float(float f);          
void OLED_Command(uint8_t command);       
void OLED_PutChar(char c);                
void OLED_Data(uint8_t data);             
void Delay_ms(int time_ms);

#endif
