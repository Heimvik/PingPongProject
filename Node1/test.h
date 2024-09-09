#pragma once
#include "drivers/uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/adc.h"

void UartTx();

void UartRxTx();


void SramTest();

void AdcTest();