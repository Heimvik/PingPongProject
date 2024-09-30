#pragma once
#include <avr/io.h>
#include "mcp2515.h"
#include "spi.h"
#include <stdio.h>
#include <util/delay.h>
#include "../defines.h"


#define CAN_CONTROLLER_IDENTIFIER 0x0
#define DATA_BYTES 0x8


void CanControllerBitModify(uint8_t addr, uint8_t data, uint8_t mask);

void CanControllerInit();

void CanControllerReset();

void CanControlllerSetMode(uint8_t mode);

uint8_t CanControllerRead(uint8_t addr);

void CanControllerWrite(uint8_t addr, uint8_t data);

void CanControllerWriteMultipleBytes(uint8_t addr, uint8_t dataLength, uint8_t* data);

void CanControllerRTS();
