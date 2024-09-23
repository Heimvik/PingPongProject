#pragma once
#include <avr/io.h>
#include "mcp2515.h"
#include "spi.h"
#include <stdio.h>
#include <util/delay.h>

#define CAN_CONTROLLER_IDENTIFIER 0x0
#define DLC 0x8

void CanControllerInit();

void CanControllerReset();


uint8_t CanControllerRead(uint8_t addr);
void CanControllerWrite(uint8_t addr, uint8_t data);
