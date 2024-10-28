#pragma once

#include <stdint.h>
#include <time.h>
#include "adc.h"

// Minimum time between goals in ms
#define MIN_GOAL_DELAY 1000

uint16_t checkAndReturnGoals();
