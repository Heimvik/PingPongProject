#pragma once
#include "drivers/uart.h"
#include <avr/io.h>
#include <util/delay.h>


void UartTx();

void UartRxTx();

void LatchTest();

void SramTest(void);
