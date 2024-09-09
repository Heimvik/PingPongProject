#include "adc.h"

volatile char *adc = (char *) 0x1400;
volatile char adcData;

void AdcInit(){
    //enable pmw clock
    //set data direction register, port D pin 5 to output
    set_bit(DDRD,PD5);
    set_bit(TCCR0,WGM01,COM00);
    clear_bit(TCCR0,WGM00,COM01);
    OCR0=0x00;

}


void AdcRead(){

    
}   