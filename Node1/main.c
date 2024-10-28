#include <stdio.h>
#include "test.h"
#include  <avr/io.h>
#include <stdlib.h>
#include "drivers/uart.h"
#include "drivers/xmem.h"
#include "drivers/adc.h"
#include "drivers/spi.h"
#include "drivers/cancontroller.h"
#include "drivers/can.h"
#include "drivers/oled.h"



int main(void) 
{
    InitUart();
    BindStdIOToUart();
    InitXmem();
    InitADC();
    printf("Hello world\n");
    SPIInit();
    CanInit();
    struct slideOfJoy_t joystick;
    uint8_t started = 0;

    OledInit();
    OledReset();
    uint8_t menures = menuSelect();
    if (menures != 1){
        printf("No bueno");
        return 0;
    }
    OledReset();
    printf("Menu selected\n");
    while(1)
    {
        if (yesWeCanFlag)
        {
            yesWeCanFlag = 0;
            struct canDataFrame_t dataFrame = CanReceive();
            if (dataFrame.id == 0x02)
            {
                uint8_t goals=dataFrame.data[0];
                displayGoals(goals);
            }
        }
        joystick = ReadADC();
        CANSendJoystick(&joystick);
        _delay_ms(50);
    }



    return 0;
}