#include <stdio.h>
#include "test.h"
#include  <avr/io.h>
#include <stdlib.h>
#include "drivers/uart.h"
#include "drivers/xmem.h"


int main(void) {
    UartInit();
    BindStdIOToUart();
    initXmem();
    AdcTest();    

    return 0;
}