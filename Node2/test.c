#include "test.h"

void TestPwm(){
    initPwm();
}

void TestAdc(){
    initAdc();
    while(1){
        readAdc();
        //printf("ADC: %d\n\r", readAdc());
        if(readAdc() < 1069){
            printf("Ball detected\n\r");
        } else {
            printf("Nothing...\n\r");
        }
    }
}

void TestGoalCounter()
{
    initAdc();
    while(1)
    {
        printf("%d\n\r", checkAndReturnGoals());
    }
}


void TestMotorPosition()
{
    initEncoder2();
    while(1)
    {
        printf("Position: %d\n\r",readEncoder());
    }
}

void TestSolenoid()
{
    InitSolenoid();
    ActivateSolenoid();
}