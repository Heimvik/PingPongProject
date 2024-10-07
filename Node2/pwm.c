#include "pwm.h"

void initPwm(){
    // Enable the peripheral clock to the PMC
    //PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_CMD | (ID_PWM << PMC_PCR_PID_Pos);
	//PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

    REG_PMC_PCER1 |= PMC_PCER1_PID36;

    REG_PIOC_PDR |= PIO_PDR_P13;
    REG_PIOC_ABSR |= PIO_ABSR_P13;
    // Enable user interface
    REG_PWM_WPCR &= ~(0x3);
    // Enable clock
    REG_PWM_CLK = 1 << PWM_CLK_DIVB_Pos;
    // Set the channel to 1
    REG_PWM_CMR1 &= ~(0xF);
    // Set the period to 20 ms
    REG_PWM_CPRD1 = 1680000;
    // Set the duty cycle to 1.5 ms
    REG_PWM_CDTY1 = 126000;
    // 900-2100 are allowed. IMPORTANT use PMDCDTYUPD1 to update the duty cycle after enabling channel
    REG_PWM_DIS = PWM_DIS_CHID1 & (~PWM_DIS_CHID0);
    // Update the duty cycle
    REG_PWM_SCUC = PWM_SCUC_UPDULOCK;
    REG_PWM_ENA=PWM_ENA_CHID1;
    // Enable write proteaction
    REG_PWM_WPCR |= 0x3;

    /*
    REG_PMC_PCER1 |= PMC_PCER1_PID36;
    REG_PIOC_PDR |= PIO_PDR_P13;
    REG_PIOC_ABSR |= PIO_ABSR_P13;
    REG_PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_DIVB(42);
    REG_PWM_CMR1 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB;
    REG_PWM_CPRD1 = 20000;
    REG_PWM_CDTY1 = 1500;
    REG_PWM_ENA = PWM_ENA_CHID1;
    */
    printf("PWM initialized\n");
}

void setDutyCycle(uint32_t dutyCycle){
    REG_PWM_CDTYUPD1 = dutyCycle;
    REG_PWM_DIS = PWM_DIS_CHID1;
    REG_PWM_ENA = PWM_ENA_CHID1;
}