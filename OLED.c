#include "OLED.h"
#include "I2C.h"
#include <string.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

static uint8_t oled_buffer[128 * 64 / 8]; // 1024 bytes for 128x64 screen

void OLED_Init() {
    I2C3_Write(0x3C, 0x00, 0xAE); // Display OFF
    I2C3_Write(0x3C, 0x00, 0xD5); // Set display clock divide ratio
    I2C3_Write(0x3C, 0x00, 0x80); // Suggested ratio
    I2C3_Write(0x3C, 0x00, 0xA8); // Set multiplex ratio
    I2C3_Write(0x3C, 0x00, 0x1F); // Set to 32 rows (for 128x32)
    I2C3_Write(0x3C, 0x00, 0xD3); // Set display offset
    I2C3_Write(0x3C, 0x00, 0x00); // No offset
    I2C3_Write(0x3C, 0x00, 0x40); // Set start line address
    I2C3_Write(0x3C, 0x00, 0x8D); // Enable charge pump regulator
    I2C3_Write(0x3C, 0x00, 0x14); //
    I2C3_Write(0x3C, 0x00, 0x20); // Memory mode
    I2C3_Write(0x3C, 0x00, 0x00); // Horizontal addressing mode
    I2C3_Write(0x3C, 0x00, 0xA1); // Set segment remap
    I2C3_Write(0x3C, 0x00, 0xC8); // Set COM scan direction
    I2C3_Write(0x3C, 0x00, 0xDA); // Set COM pins hardware configuration
    I2C3_Write(0x3C, 0x00, 0x02); //
    I2C3_Write(0x3C, 0x00, 0x81); // Set contrast control
    I2C3_Write(0x3C, 0x00, 0x8F); //
    I2C3_Write(0x3C, 0x00, 0xD9); // Set pre-charge period
    I2C3_Write(0x3C, 0x00, 0xF1); //
    I2C3_Write(0x3C, 0x00, 0xDB); // Set VCOMH deselect level
    I2C3_Write(0x3C, 0x00, 0x40); //
    I2C3_Write(0x3C, 0x00, 0xA4); // Resume to RAM content display
    I2C3_Write(0x3C, 0x00, 0xA6); // Normal display mode
    I2C3_Write(0x3C, 0x00, 0xAF); // Display ON
}

void OLED_Clear(void) {
    for (uint8_t row = 0; row < 8; row++) {
        OLED_YX(row, 0);
        for (uint8_t col = 0; col < 128; col++) {
            OLED_Data(0x00); 
        }
    }
}

void OLED_Update() {
    for (uint8_t page = 0; page < 4; page++) { 
        I2C3_Write(0x3C, 0x00, 0xB0 + page); 
        I2C3_Write(0x3C, 0x00, 0x00);       
        I2C3_Write(0x3C, 0x00, 0x10);      
        for (uint8_t col = 0; col < SCREEN_WIDTH; col++) {
            I2C3_Write(0x3C, 0x40, oled_buffer[page * SCREEN_WIDTH + col]);
        }
    }
}

void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return; 
    uint16_t index = x + (y / 8) * SCREEN_WIDTH;         
    if (color)
        oled_buffer[index] |= (1 << (y % 8));         
    else
        oled_buffer[index] &= ~(1 << (y % 8));         
}

void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
    for (uint8_t i = 0; i < width; i++) {
        OLED_DrawPixel(x + i, y, color);
        OLED_DrawPixel(x + i, y + height - 1, color);
    }
    for (uint8_t i = 0; i < height; i++) {
        OLED_DrawPixel(x, y + i, color);
        OLED_DrawPixel(x + width - 1, y + i, color);
    }
}
void Test_Screen() {
    OLED_Clear();
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
            OLED_DrawPixel(x, y, 1); 
        }
    }
    OLED_Update();
}

void OLED_Command(uint8_t command) {
    I2C3_Write(0x3C, 0x00, command); 
}
void OLED_Data(uint8_t data) {
    I2C3_Write(0x3C, 0x40, data); 
}
extern const uint8_t OledFont[][8]; 

void OLED_PutChar(char c) {
    if (c < 32 || c > 127) {
        c = ' '; 
    }

    const uint8_t *font = OledFont[c - 32]; 
    for (uint8_t i = 0; i < 8; i++) {
        OLED_Data(font[i]);
    }
}

void OLED_YX(uint8_t row, uint8_t column) {
    OLED_Command(0xB0 + row); 
    OLED_Command(0x00 + (8 * column & 0x0F));
    OLED_Command(0x10 + ((8 * column >> 4) & 0x0F)); 
}

void OLED_Write_String(const char *str) {
    while (*str) {
        OLED_PutChar(*str++);
    }
}

void Delay_ms(int time_ms) {
    int i, j;
    for (i = 0; i < time_ms; i++) {
        for (j = 0; j < 3180; j++) {
            __asm("NOP"); 
        }
    }
}