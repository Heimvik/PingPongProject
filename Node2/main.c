#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "drivers/can_controller.h"
#include "drivers/time.h"
#include "sam.h"
#include "test.h"
#include "drivers/userInput.h"
#include "drivers/servo.h"
#include "drivers/pwm.h"
#include "drivers/adc.h"


#define DEBUG_INTERRUPT 0
CAN_MESSAGE message;

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}


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
    

    TestAdc();

    TestPwm();
    struct slideOfJoy_t joyPos;

    while (1)
    {
        //*
        joyPos.xJoy = 0.3 * joyPos.xJoy + 0.7 * (message.data[0] - 128);
        joyPos.yJoy = 0.3 * joyPos.yJoy + 0.7 * message.data[1] - 128;
        joyPos.sliderLeft = 0.3 * joyPos.sliderLeft + 0.7 * message.data[3];
        joyPos.sliderRight = 0.3 * joyPos.sliderRight + 0.7 * message.data[4];
        printf("%d %d %d %d\r\n", joyPos.xJoy, joyPos.yJoy, joyPos.sliderLeft, joyPos.sliderRight);
        //*/

        //setServoPosFromUint8(joyPos.sliderRight);
        setServoPosFromInt8(joyPos.xJoy);
    }
}