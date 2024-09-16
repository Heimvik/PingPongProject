#include "oled.h"

volatile uint8_t* fbPtr = (volatile uint8_t*)SRAM_ADDR; // 1024 bytes for this
volatile uint8_t* oledCommandPtr = (volatile uint8_t*)OLED_COMMAND_ADDR;
volatile uint8_t* oledDataPtr = (volatile uint8_t*)OLED_DATA_ADDR;


void OledInit() 
{
    // Turn on display
    *oledCommandPtr = 0xAF;
    // Horizontal address mode
    *oledCommandPtr = 0x20;
    *oledCommandPtr = 0b00;
    // Maximum contrast
    *oledCommandPtr = 0x81;
    *oledCommandPtr = 0xFF;
    // Set page address
    *oledCommandPtr = 0x22;
    *oledCommandPtr = 0x00;
    *oledCommandPtr = 0x07;
    // Set column address
    *oledCommandPtr = 0x21;
    *oledCommandPtr = 0x00;
    *oledCommandPtr = 0x7F;
    // Flip display
    *oledCommandPtr = 0xC8;
    *oledCommandPtr = 0xA1;
}


void OledReset()
{
    for (int i = 0; i < 1024; ++i)
    {
        fbPtr[i] = 0;
        *oledDataPtr = 0;
    }
}


void OledClearPixel(uint8_t row, uint8_t column) 
{
    uint16_t index = (row/8)*128 + column;
    uint8_t bitOffset = row % 8;
    clear_bit(fbPtr[index], bitOffset);
}


void OledSetPixel(uint8_t row, uint8_t column) 
{
    uint16_t index = (row/8)*128 + column;
    uint8_t bitOffset = row % 8;
    set_bit(fbPtr[index], bitOffset);
}


void OledWriteOutFb() 
{
    for (int i = 0; i < 1024; ++i)
    {
        *oledDataPtr = fbPtr[i];
    }
}


void OledPrintLn(uint8_t line, char* input)
{
    uint8_t endOfString = 0;
    uint8_t charIndex = 0;
    for (uint8_t i = 0; i < 128; ++i)
    {
        // Assume null terminated string
        if (input[charIndex] == 0x00)
        {
            fbPtr[128*line + i] = 0x00;
        }
        else
        {
            for (uint8_t j = 0; j < 5; ++j) {
                fbPtr[128*line + i] = font5[input[charIndex]][j];
                ++i;
            }
            ++charIndex;
        }
    }
}
