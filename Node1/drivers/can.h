#pragma once

#include <stdint.h>
#include "cancontroller.h"
#include <avr/interrupt.h>

extern uint8_t yesWeCanFlag;

struct canDataFrame_t {
    uint8_t id;
    uint8_t length;
    uint8_t data[8];
};


void CanSend(struct canDataFrame_t* dataFrame);

struct canDataFrame_t CanReceive();

void CanInit();