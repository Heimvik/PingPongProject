#pragma once

#include <stdint.h>

typedef enum
{
    UP,DOWN,LEFT,RIGHT, NEUTRAL
} joyDirection_t;

struct slideOfJoy_t
{
    int8_t yJoy, xJoy;
    uint8_t sliderLeft, sliderRight;
    joyDirection_t joyDirection;
};
