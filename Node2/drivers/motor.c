#include "motor.h"

void initEncoder(){
    initMotor();
    setMotorDirection(0);
    setMotorDutyCycle(30);
    time_spinFor(seconds(1));
    setMotorDutyCycle(0);
    printf("Encoder init\n\r");
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
    //initTimerInterrupt();

}

uint32_t readEncoder(){
    return (uint32_t)REG_TC2_CV0;
}



void initMotor(){
    //
    REG_PIOC_OER |= PIO_OER_P23;
    initPwm();
    //initTimerInterrupt();
}

void setMotorDirection(uint8_t dir){
    if (dir){
        REG_PIOC_CODR = PIO_CODR_P23;
    } else {
        REG_PIOC_SODR = PIO_SODR_P23;
    }
}



void TC1_Handler(void){
    while (1)
    {
        printf("Interrupt\n\r");
    }
    
}

void initTimerInterrupt(){
    __enable_irq();

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
    while (1)
    {
        printf("counter value: %d\n\r", REG_TC1_CV0);
    }
    


}





int32_t integralerror = 0;
int32_t wantedPosition = 0;

float Kp = 0.02;
float Ki = 0.002;

float PIcontroller(int32_t wantedPosition, uint8_t T) {
    uint32_t position = readEncoder();
    int32_t error = wantedPosition - (int32_t)position;

    integralerror += error;
    printf("Error: %d\n\r", error);
    printf("Integral error: %d\n\r", integralerror);

    if (integralerror > 6000) {
        integralerror = 6000;
    } else if (integralerror < -6000) {
        integralerror = -6000;
    }

    float controlSignal = (float)(Kp * error + Ki * integralerror);

    // Convert float to int for printing
    if (controlSignal < 0) {
        setMotorDirection(0);  // Reverse direction
        controlSignal = -controlSignal;
    } else {
        setMotorDirection(1);  // Forward direction
    }

    if (controlSignal > 100) {
        controlSignal = 100;
    }

    setMotorDutyCycle((uint32_t)controlSignal);
    return controlSignal;
}
