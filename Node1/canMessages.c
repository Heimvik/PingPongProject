#include "canMessages.h"


void CANSendJoystick(struct slideOfJoy_t* data)
{
    struct canDataFrame_t dataFrame;
    uint16_t temp;

    dataFrame.id = 0;
    dataFrame.dataLength = 5;
    temp = data->xJoy + 128;
    dataFrame.data[0] = (uint8_t)temp;
    temp = data->yJoy + 128;
    dataFrame.data[1] = (uint8_t)temp;
    dataFrame.data[2] = (uint8_t)data->joyDirection;
    dataFrame.data[3] = data->sliderLeft;
    dataFrame.data[4] = data->sliderRight;

    CanSend(&dataFrame);
}
