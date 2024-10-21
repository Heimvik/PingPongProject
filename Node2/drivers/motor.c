#include "motor.h"

void initEncoder(){
    REG_PMC_PCER1 |= PMC_PCER1_PID33 | PMC_PCER1_PID34 | PMC_PCER1_PID35;

    // PC25 setup
    REG_PIOC_OER &= ~(PIO_OER_P25);
    // diable PIO control
    REG_PIOC_PDR |= PIO_PDR_P25;
    //peripheral B
    REG_PIOC_ABSR |= PIO_ABSR_P25;

    //PC26 setup
    //input enable
    REG_PIOC_OER &= ~(PIO_OER_P26);
    //disable PIO control
    REG_PIOC_PDR |= PIO_PDR_P26;
    //peripheral B
    REG_PIOC_ABSR |= PIO_ABSR_P26;

    //enable counter clock channel 0
    REG_TC2_CCR0 = TC_CCR_CLKEN;
    // channel mode reg xc0 clock selection
    REG_TC2_CMR0 |= TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;
    REG_TC2_BMR |= TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_TC0XC0S_TCLK0;

    REG_TC2_CMR0 &= ~(TC_CMR_WAVE);

    printf("TC2 status: %x\n\r", REG_TC2_SR0);  
    //2828

}

uint32_t readEncoder(){
    return (uint32_t)REG_TC2_CV0;
}



void initMotor(){
    //
    REG_PIOC_OER |= PIO_OER_P23;
    initPwm();
    initTimerInterrupt();
}

void setMotorDirection(uint8_t dir){
    if (dir){
        REG_PIOC_CODR = PIO_CODR_P23;
    } else {
        REG_PIOC_SODR = PIO_SODR_P23;
    }
}



void TC1_Handler(void){
    __disable_irq();
    while (1)
    {
        printf("Interrupt\n\r");
    }
    
}

void initTimerInterrupt(){
    printf("Init timer interrupt\n\r");
    REG_PMC_PCER0 |= PMC_PCER0_PID30 | PMC_PCER0_PID31;
    REG_PMC_PCER1 |= PMC_PCER1_PID32; 
    
    REG_TC1_CMR0= TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
    REG_TC1_RC0 = 1000000000;

    REG_TC1_IER0 = 0x11;
    NVIC_EnableIRQ(TC1_IRQn);



    REG_TC1_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    printf("status: %x\n\r", REG_TC1_SR0);
    printf("interrupt status %x\n\r", REG_TC1_IMR0);
    __enable_irq();


}


uint8_t Kp = 1;
uint8_t Ki = 1;

uint16_t T = 1000;

uint16_t error = 0;

uint8_t wantedPosition = 0;

void PIcontroller(){

    
}