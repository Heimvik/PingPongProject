#include "oled.h"

uint8_t fb[1024];
volatile uint8_t* oledCommandPtr = (volatile uint8_t*)OLED_COMMAND_ADDR;
volatile uint8_t* oledDataPtr = (volatile uint8_t*)OLED_DATA_ADDR;

void OledInit() {
    // Horizontal address mode
    oledCommandPtr = 0x20;
    oledCommandPtr = 0b01;
    // Maximum contrast
    oledCommandPtr = 0x81;
    oledCommandPtr = 0xFF;
}

void OledClearPixel(uint8_t row, uint8_t column) {
    uint8_t index = (row/8)*128 + column;
    uint8_t bitOffset = row % 8;
    //DEBUG
    printf("Oled clear: %d %d", index, bitOffset);
    fb[index] &= ~(1 << bitOffset);
}

void OledSetPixel(uint8_t row, uint8_t column) {
    uint8_t index = (row/8)*128 + column;
    uint8_t bitOffset = row % 8;
    //DEBUG
    printf("Oled set: %d %d", index, bitOffset);
    fb[index] |= 1 << bitOffset;
}

void OledWriteOutFb() {
    // Make sure page and column are a 0


}
