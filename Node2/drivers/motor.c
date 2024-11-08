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
    initTimerInterrupt();

}

double readEncoder(){
    uint32_t encoderValue = REG_TC2_CV0;
    if (encoderValue > ENCODER_MAX_VALUE*2){
        encoderValue = 0;
    }
    double percentage = ((double)encoderValue * 100) / ENCODER_MAX_VALUE;

    return percentage;
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

volatile uint32_t a;

void TC3_Handler(void){
    //PIcontroller();
    PIcontroller();
    NVIC_ClearPendingIRQ(TC3_IRQn);
    a=REG_TC1_SR0;
}


void initTimerInterrupt(){
    __enable_irq();
    NVIC_ClearPendingIRQ(TC3_IRQn);

    NVIC_EnableIRQ(TC3_IRQn);
    REG_TC1_IER0 = 0x10;

    printf("Init timer interrupt\n\r");
    REG_PMC_PCER0 |= PMC_PCER0_PID30 | PMC_PCER0_PID31;
    REG_PMC_PCER1 |= PMC_PCER1_PID32; 
    
    REG_TC1_CMR0= TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
    REG_TC1_RC0 = 2100000;




    REG_TC1_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    printf("status: %x\n\r", REG_TC1_SR0);
    printf("interrupt status %x\n\r", REG_TC1_IMR0);

}





double integralerror = 0;
double wantedPosition = 0;
double prevError = 0;

double Kp = 0.5;
double Ki = 0.3;
double Kd = 0.1;
double dt=0.05;

void setReferencePosition(int32_t position){
    wantedPosition = position;
}

void PIcontroller() {
    double position = readEncoder();
    double error = (double)wantedPosition - (double)position;

    integralerror = integralerror + error*dt;


    double derivative = -(error - prevError)/dt;


    double controlSignal = (double)(Kp * error + Ki * integralerror+Kd*derivative);


    // Convert double to int for printing
    if (controlSignal < 0) {
        setMotorDirection(0);  // Reverse direction
        controlSignal = -controlSignal;
    } else {
        setMotorDirection(1);  // Forward direction
    }

    if (controlSignal > 150) {
        controlSignal = 150;
    }
    prevError=error;

    setMotorDutyCycle((uint32_t)controlSignal);

}
