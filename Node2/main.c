#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can.h"
#include "sam.h"
#include "time.h"


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    configure_uart();
    printf("Hello World\n\r");
    CanInit init = { .brp = 0x14, .phase1 = 0x2, .phase2 = 0x2, .propag = 0x1, .sjw = 0x1, .smp = 0x0 };

    can_init(init, 1);
    CanMsg m;
    while (1)
    {
        printf("Checking for message\n\r");
        if (can_rx(&m)){
            printf("Ohmyfuckinggod\n\r");
            can_printmsg(m);
        }
        time_spinFor(msecs(200));

        //can_tx((CanMsg){.id = 0x01, .length = 8, .byte8 = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}});
    }
    
}