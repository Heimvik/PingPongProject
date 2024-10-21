#pragma once    
#include "sam.h"
#include "time.h"
#include "pwm.h"
void initEncoder();


uint32_t readEncoder();

void initMotor();

void setMotorDirection(uint8_t dir);

float PIcontroller(int32_t wantedPosition, uint8_t T);

