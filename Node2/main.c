#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "time.h"
#include "sam.h"
#include "test.h"


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    configure_uart();
    printf("Hello World\n\r");
    //CanInit init = { .phase2 = 0x5, .propag = 0x0, .phase1 = 0x6, .sjw = 0x0, .brp = 41, .smp = 0x0 };

    can_init_def_tx_rx_mb(41 << 16 | 0x6 << 4 | 0x5);
    //CanMsg m;
    //can_tx((CanMsg){.id = 0x01, .length = 8, .byte8 = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}});
    
    printf("Configuration:\n");
    uint32_t canBr = CAN0->CAN_BR;
    printf("CAN_BR: %x\n\r", canBr);

    uint32_t canMr = CAN0->CAN_MR;
    uint32_t canSr = CAN0->CAN_SR;
    printf("CAN_MR: %x\n\rCAN_SR: %x\n\r", canMr, canSr);

    uint32_t canEcr = CAN0->CAN_ECR;
    uint32_t canIer = CAN0->CAN_IER;
    uint32_t canIdt = CAN0->CAN_IDR;
    printf("CAN_ECR: %x\n\rCAN_IER: %x\n\rCAN_IDR: %x\n\r", canEcr, canIer, canIdt);

    uint32_t error = 0;
    
    //NVIC->ICER[0] = 0xFFFFFFFF; // Disable all interrupts

    TestPwm();
    while (1)
    {
        // printf("Checking for message\n\r");
        // if (can_rx(&m)){
        //     printf("Ohmyfuckinggod\n\r");
        //     can_printmsg(m);
        // }

        // error = CAN0->CAN_ECR;

        // canSr = CAN0->CAN_SR;
        // printf("TEC: %2x\t REC: %2x\t SR: %8x\n\r", error>>16, error & 0xFF,canSr);

        time_spinFor(1);
        //can_tx((CanMsg){.id = 0x01, .length = 8, .byte8 = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}});
    }
}