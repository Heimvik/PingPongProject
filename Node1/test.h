#pragma once
#include "drivers/uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/adc.h"
#include "drivers/oled.h"


void TestUartTx();

void TestUartRxTx();

void TestSram();

void TestADC();

void TestOled();