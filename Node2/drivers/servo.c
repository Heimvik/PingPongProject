#include "servo.h"

void setServoPosFromUint8(uint8_t value)
{
    // PWM Driver is defined to use microseconds
    // Should be limited to 0.9 ms - 2.1 ms based on input
    uint32_t dutyCycleUs = 900 + (255 - value) * ((2100 - 900) / 255);
    dutyCycleUs = min(dutyCycleUs, 2100);
    dutyCycleUs = max(dutyCycleUs, 900);
    setDutyCycle(dutyCycleUs);
}

void setServoPosFromInt8(int8_t value)
{
    // PWM Driver is defined to use microseconds
    // Should be limited to 0.8 ms - 2.1 ms based on input
    uint32_t dutyCycleUs = 1500 - value * ((2100 - 1500) / 128);
    dutyCycleUs = min(dutyCycleUs, 2100);
    dutyCycleUs = max(dutyCycleUs, 900);
    setDutyCycle(dutyCycleUs);
}
