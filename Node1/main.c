#include <stdio.h>
#include "test.h"
#include  <avr/io.h>
#include <stdlib.h>
#include "drivers/uart.h"


int main(void) {
    UartInit();
    BindStdIOToUart();
    SramTest();
    //LatchTest();

    return 0;
}