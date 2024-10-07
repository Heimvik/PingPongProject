#include "canMessages.h"


void CANSendJoystick(int8_t xPos, int8_t yPos)
{
    struct canDataFrame_t dataFrame;
    uint16_t temp;

    dataFrame.id = 0;
    dataFrame.dataLength = 2;
    temp = xPos + 128;
    dataFrame.data[0] = (uint8_t)temp;
    temp = yPos + 128;
    dataFrame.data[1] = (uint8_t)temp;

    CanSend(&dataFrame);
}
