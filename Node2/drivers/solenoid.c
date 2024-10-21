#include "drivers/solenoid.h"

uint8_t SolenoidActive = 0;

void InitSolenoid(){
    //Enable PIO clock
    REG_PMC_PCER0 |= PMC_PCER0_PID11;
    //Set pin as output
    REG_PIOA_OER |= PIO_OER_P29;
}

void ActivateSolenoid(){
    if(!SolenoidActive){
        //REG_PIOC_CODR = PIO
    }
}