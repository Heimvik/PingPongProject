#include "drivers/solenoid.h"

uint8_t SolenoidActive = 0;

void InitSolenoid(){
    //Enable PIO clock
    REG_PMC_PCER0 |= PMC_PCER0_PID13;
    //Set pin as output
    REG_PIOC_OER |= PIO_OER_P9;
}

void ActivateSolenoid(){
    printf("Activating solenoid\n\r");
    REG_PIOC_SODR = PIO_SODR_P9;
    time_spinFor(10000000);
    REG_PIOC_CODR = PIO_CODR_P9;
    printf("Deactivating solenoid\n\r");

}