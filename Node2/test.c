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