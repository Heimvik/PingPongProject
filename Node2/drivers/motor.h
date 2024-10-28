#pragma once    
#include "sam.h"
#include "time.h"
#include "pwm.h"
void initEncoder();
#define ENCODER_MAX_VALUE 2833


void initMotor();

void setMotorDirection(uint8_t dir);

void PIcontroller();

void setReferencePosition(int32_t position);

double readEncoder();