#pragma once
#include <avr/io.h>
#include "../defines.h"
#include <stdio.h>


void UartInit();

void BindStdIOToUart();

void UartTransmit(unsigned char data);

unsigned char UartReceive( );

