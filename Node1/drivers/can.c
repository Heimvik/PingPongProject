#include "can.h"
uint8_t yesWeCanFlag = 0;

ISR(INT2_vect)
{
    //NOTE: I hate this name :)
    // Change we can believe in
    cli();
    yesWeCanFlag = 1;
    sei();
}

void CanInit()
{
    cli();

    CanControllerInit();

    // Enable interrupt on INT2        
    set_bit(SREG, 7);
    clear_bit(EMCUCR, ISC2);
    set_bit(GICR, INT2);
    
    
   sei();
}


void CanSend(struct canDataFrame_t* dataFrame)
{
    CanControllerWrite(MCP_TXB2SIDL, (dataFrame->id)<<5);
    CanControllerWrite(MCP_TXB2SIDH, (dataFrame->id)>>3);
    CanControllerWrite(MCP_TXB2DLC, dataFrame->length);
    // Load data into TX bufferSREG
    CanControllerWriteMultipleBytes(MCP_TXB2D0, 8, dataFrame->data);
    // Ready to Send buff 0
    CanControllerRTS(0);
    printf("Message sent\n");
}


struct canDataFrame_t CanReceive()
{

    uint8_t canintf = CanControllerRead(MCP_CANINTF);
    struct canDataFrame_t dataFrame;
    uint8_t receiveBaseAddress;
    
    if (canintf & (0xE0))
    {
        printf("Error: Received bad can interrupt %x\n", canintf);
        CanControllerWrite(MCP_CANINTF, 0x00);
        return;
    }
    
    else if (canintf & MCP_RX0IF)
    {
        receiveBaseAddress = MCP_RXB0CTRL;
        CanControllerBitModify(MCP_CANINTF, 0x00, 0x01);
    }
    else if (canintf & MCP_RX1IF)
    {
        receiveBaseAddress = MCP_RXB1CTRL;
        CanControllerBitModify(MCP_CANINTF, 0x00, 0x02);
    }
    else
    {
        printf("Error: Did not receive any message\n");
        return;
    }
    
    dataFrame.id = 0;
    dataFrame.id |= CanControllerRead(receiveBaseAddress + 1)<<3;
    dataFrame.id |= CanControllerRead(receiveBaseAddress + 2)>>5;
    dataFrame.length = CanControllerRead(receiveBaseAddress + 5);
    for (uint8_t i = 0; i < dataFrame.length; ++i)
    {
        dataFrame.data[i] = CanControllerRead(receiveBaseAddress + 6 + i);
    }

    return dataFrame;
}
