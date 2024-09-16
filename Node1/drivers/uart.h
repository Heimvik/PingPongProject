#pragma once
#include <avr/io.h>
#include "../defines.h"
#include <stdio.h>


void InitUart();

void BindStdIOToUart();

void TransmitUart(unsigned char data);

unsigned char ReceiveUart();
