#include <stdio.h>
#include "drivers/uart.h"



int main(void) {
    UartInit();
    BindStdIOToUart();


    while (1)
    {
        printf("Hello World\n");
    }
    
    return 0;
}