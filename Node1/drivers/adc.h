#pragma once
#include <avr/io.h>
#include "../defines.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>


typedef enum{
    UP,DOWN,LEFT,RIGHT, NEUTRAL
} joyDirection_t;

struct slideOfJoy_t
{
    int8_t yJoy, xJoy;
    uint8_t sliderLeft, sliderRight;
    joyDirection_t joyDirection;
};

void InitADC(void);


struct slideOfJoy_t ReadADC();