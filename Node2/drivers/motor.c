#include "motor.h"


void initEncoder(){
    // PMC
    REG_PMC_PCER1 |= PMC_PCER1_PID33 | PMC_PCER1_PID34 | PMC_PCER1_PID35;

    // PC25 setup
    //Enable Timer/Counter channel 0 clock
    REG_PMC_PCER0 |= PMC_PCER0_PID27;
    // input enable
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

    //PB26 setup for clock input
    REG_PIOB_PDR |= PIO_PDR_P26;
    REG_PIOB_ABSR |= PIO_ABSR_P26;

  
    REG_TC2_CCR0 = TC_CCR_CLKEN;
    // Set TC0XC0S to TCKL0
    REG_TC0_BMR &= ~(0b11);
    REG_TC0_WPMR=0x54494D00;
    
    /*Quadrature decoder mode requires setting the block mode register, which will
    change the operating mode of the entire TC2 instance. You must also set XC0 as the
    selected clock for channel 0.*/
    //Disable write protection  
    
    //Select external clock XC0 for channel 0
    REG_TC2_BMR |= TC_BMR_QDEN | TC_BMR_POSEN;
    // Set XC0 as selected clock for channel +0
    REG_TC2_CMR0 |= TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG; 
    REG_TC2_CMR0 &= ~(TC_CMR_WAVE);
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
    printf("Interrupt\n\r");
}

void initTimerInterrupt(){
    printf("Init timer interrupt\n\r");
    REG_PMC_PCER0 |= PMC_PCER0_PID30 | PMC_PCER0_PID31;
    REG_PMC_PCER1 |= PMC_PCER1_PID32; 
    
    REG_TC1_CMR0= TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
    REG_TC1_RC0 = 1000;

    REG_TC1_IER0 = TC_IER_CPCS;
    NVIC_EnableIRQ(TC1_IRQn);



    REG_TC1_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    printf("status: %x\n\r", REG_TC1_SR0);


}

void PIcontroller(){
    
}