#include "pwm.h"

void initPwm()
{
    // Enable the peripheral clock to the PMC
    //PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_CMD | (ID_PWM << PMC_PCR_PID_Pos);
	//PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
    printf("Entered init PWM\n\r");

    REG_PMC_PCER1 |= PMC_PCER1_PID36;

    REG_PIOB_OER |= PIO_OER_P12;
    REG_PIOB_PDR |= PIO_PDR_P12;
    REG_PIOB_ABSR |= PIO_ABSR_P12;
    
    REG_PIOB_OER |= PIO_OER_P13;
    REG_PIOB_PDR |= PIO_PDR_P13;
    REG_PIOB_ABSR |= PIO_ABSR_P13;

    // Disable write protection on all groups
    REG_PWM_WPCR = 0x50574DFC;
    printf("PWM_WPSR %x\n\r", REG_PWM_WPSR);
    printf("PWM_SCM: %x\n\r", REG_PWM_SCM);
    // Enable clock
    REG_PWM_CLK = PWM_CLK_DIVB(84) | PWM_CLK_PREB(0);

    //Servo setup
    // Use clk B, with polarity 1
    REG_PWM_CMR1 = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB;
    // Set the period to 20 ms
    printf("PWM_CMR1: %x\n\r", REG_PWM_CMR1);
    REG_PWM_CPRD1 = 20000;
    // 900-2100 are allowed. IMPORTANT use PMDCDTYUPD1 to update the duty cycle after enabling channel
    REG_PWM_CDTY1 = 1500;

    
    //REG_PWM_CDTYUPD1 = 
    // Update the duty cycle
    REG_PWM_ENA = PWM_ENA_CHID1;

    ///Motor setup
    REG_PWM_CMR0 = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB;
    // Set the period to 20 ms
    REG_PWM_CPRD0 = 100;
    // 900-2100 are allowed. IMPORTANT use PMDCDTYUPD1 to update the duty cycle after enabling channel
    // Set the duty cycle to 1.5 ms
    REG_PWM_CDTY0 = 0;
    REG_PWM_ENA = PWM_ENA_CHID0 | PWM_ENA_CHID1;
    

    printf("PWM initialized\n");
}

void setDutyCycle(uint32_t dutyCycle)
{
    // Kanskje bare CDTY1
    REG_PWM_CDTYUPD1 = dutyCycle;
}

void setMotorDutyCycle(uint32_t dutyCycle){
    REG_PWM_CDTYUPD0 = dutyCycle;
}