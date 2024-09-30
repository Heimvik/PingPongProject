#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../defines.h"
#include <stdio.h>


uint8_t SPITranceive(uint8_t cData);

void SPIInit();

void SPISetSlaveSelect(uint8_t value);
