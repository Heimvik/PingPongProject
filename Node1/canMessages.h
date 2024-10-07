#pragma once

#include <stdint.h>
#include "drivers/adc.h"
#include "drivers/can.h"


void CANSendJoystick(struct slideOfJoy_t* data);
