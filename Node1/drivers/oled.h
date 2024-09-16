#pragma once
#include <stdint.h>
//#include "fonts.h"
#include "../defines.h"
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#define OLED_COMMAND_ADDR 0x1000
#define OLED_DATA_ADDR 0x1200


void OledInit();

void OledReset();

void OledClearPixel(uint8_t row, uint8_t column);

void OledSetPixel(uint8_t row, uint8_t column);

void OledWriteOutFb();

void OledPrintLn(uint8_t line, char* input);

void OledPrintLnInverted(uint8_t line, char* input);

