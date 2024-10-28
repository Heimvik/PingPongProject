#include "adc.h"


void initAdc()
{
    // Turn off write protection
    REG_PIOA_WPMR = 0x50494F00;
    REG_ADC_WPMR = 0x41444300;

    // Enable ADC clock
    REG_PMC_PCER1 |= PMC_PCER1_PID37;
    // Disable PCM control over pin
    REG_PIOA_PDR |= PIO_PDR_P2;

    // Set up ADC clock in TRASNFER, TRACKTIME and set resolution to 12 bits
    REG_ADC_MR |= ADC_MR_TRANSFER(1) | ADC_MR_TRACKTIM(15);
    REG_ADC_MR &= ~ADC_MR_LOWRES;

    // Enable Channel 0
    REG_ADC_CHER |= ADC_CHER_CH0;

    // Set gain to 2
    REG_ADC_CGR = ADC_CGR_GAIN0(2);

    //Turn on write prtection
    REG_PIOA_WPMR = 0x50494F01;
    REG_ADC_WPMR = 0x41444301;
}

uint16_t readAdc()
{
    //Write the ADC_CR with START = 1
    REG_ADC_CR = ADC_CR_START;
    //Wait for ADC_SR DRDY bit is set
    while(!(REG_ADC_ISR & ADC_ISR_DRDY)) {}
    //Read the ADC_LCDR
    return (uint16_t)REG_ADC_LCDR;
}
