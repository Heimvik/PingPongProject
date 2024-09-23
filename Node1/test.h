#pragma once
#include "drivers/uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/adc.h"
#include "drivers/oled.h"
#include "oledMenu.h"
#include "drivers/spi.h"


void TestUartTx();

void TestUartRxTx();

void TestSram();

void TestADC();

void TestOled();

void TestMenu();

void TestSPI();