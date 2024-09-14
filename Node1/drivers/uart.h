#pragma once
#include <avr/io.h>
#include "../defines.h"
#include <stdio.h>


void InitUART();

void BindStdIOToUart();

void UartTransmit(unsigned char data);

unsigned char UartReceive( );

