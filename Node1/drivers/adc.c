#include "adc.h"

volatile unsigned char *adc = (char *) 0x1400;
uint8_t xOffset, yOffset;
int16_t yJoy, xJoy;
uint8_t sliderLeft, sliderRight;
struct slideOfJoy_t slideOfJoy;


void InitADC()
{
    //enable pmw clock
    //set data direction register, port D pin 5 to output
    set_bit(DDRD, PD5);
    set_bit(TCCR1B, CS10); // CS1 = 1 - use internal clock
    set_bit(TCCR1B, WGM13, WGM12);
    set_bit(TCCR1A, WGM11, WGM10);  // WGM1 = 15- sets Fast PWM mode
    set_bit(TCCR1A, COM1A0); // COM1A = 0b01 - sets mode to toggle
    
    adc[0x00] = 0x00;
    _delay_ms(10);

    yOffset = adc[0] ;
    xOffset = adc[0] ;
    printf("Calibrated \n xOffset: %d, yOffset: %d\n", xOffset, yOffset);
}


struct slideOfJoy_t ReadADC()
{
    adc[0] = 0;
    _delay_us(100); // theoretically could be 15 or so

    yJoy = (adc[0]-yOffset);
    if (yJoy > 0)
    {
        yJoy = yJoy * 128 / (2 * 128 - yOffset);
    }
    else
    {
        yJoy = yJoy * 128 / (yOffset);  
    }

    xJoy = (adc[0]-xOffset);
    if (xJoy > 0)
    {
        xJoy = xJoy * 128 / (2 * 128 - xOffset);
    }
    else
    {
        xJoy = xJoy * 128 / (xOffset);
    }

    slideOfJoy.xJoy = (int8_t)xJoy;
    slideOfJoy.yJoy = (int8_t)yJoy;
    slideOfJoy.sliderLeft = adc[0];
    slideOfJoy.sliderRight = adc[0];

    //joystick direction
    if (abs(xJoy) > abs(yJoy))
    {
        if (abs(xJoy) < 10)
        {
            slideOfJoy.joyDirection = NEUTRAL;
        }
        else if (xJoy > 0)
        {
            slideOfJoy.joyDirection = RIGHT;
        }
        else
        {
            slideOfJoy.joyDirection = LEFT;
        }
    }
    else
    {
        if (abs(yJoy) < 10)
        {
            slideOfJoy.joyDirection = NEUTRAL;
        }
        else if (yJoy > 0)
        {
            slideOfJoy.joyDirection = UP;
        }
        else
        {
            slideOfJoy.joyDirection = DOWN;
        }
    }
    return slideOfJoy;
}
