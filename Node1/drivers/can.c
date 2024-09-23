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
    // Enable interrupt
    
    set_bit(SREG, 7);
    clear_bit(EMCUCR, ISC2);
    set_bit(GICR, INT2);
    
    CanControllerWrite(MCP_CANINTE, 0x01);
    _delay_ms(100);
    uint8_t intf = CanControllerRead(MCP_CANINTF);
    uint8_t inte = CanControllerRead(MCP_CANINTE);
    printf("CANINTE, CANINTF: %x,%x\n", inte,intf);
    
   sei();
}


void CanSend(struct canDataFrame_t* dataFrame)
{
    // Set up ID for the message

    /*	
    unsigned int ID = message->id;
	char id_high = ID / 8;
	char id_low = ID % 8;
	id_low = id_low*0b100000;
	mcp_2515_write(MCP_TXB0SIDH + 16 * buffer_number, id_high);
	mcp_2515_write(MCP_TXB0SIDL + 16 * buffer_number, id_low);
	
	// Define data length of message
	uint8_t data_length = message->length;
	mcp_2515_write(MCP_TXB0DLC + 16 * buffer_number, data_length);
    
    char* data_bytes = message->data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		mcp_2515_write(MCP_TXB0Dm + byte + 16 * buffer_number, data_bytes[byte]);
	}
    
    */
    printf("%x %x\n", (dataFrame->id)>>3, (dataFrame->id)<<5);
    CanControllerWrite(MCP_TXB2SIDL, (dataFrame->id)<<5);
    CanControllerWrite(MCP_TXB2SIDH, (dataFrame->id)>>3);
    CanControllerWrite(MCP_TXB0DLC + 16 * 2, 8);
    // Load data into TX bufferSREG
    CanControllerWriteMultipleBytes(MCP_TXB2D0, 8, dataFrame->data);
    // Ready to Send buff 0
    CanControllerRTS(0);
    printf("Message sent\n");
}

struct canDataFrame_t CanReceive()
{
    struct canDataFrame_t dataFrame0, dataFrame1;

    // Read data from MCP_RXB0CTRL
    dataFrame0.id = 0;
    dataFrame0.id |= CanControllerRead(MCP_RXB0CTRL + 1) << 3;
    dataFrame0.id |= CanControllerRead(MCP_RXB0CTRL + 2) >> 5;
    for (uint8_t i = 0; i < 8; ++i)
    {
        dataFrame0.data[i] = CanControllerRead(MCP_RXB0CTRL + 6 + i);
    }

    // Read data from MCP_RXB1CTRL
    dataFrame1.id = 0;
    dataFrame1.id |= CanControllerRead(MCP_RXB1CTRL + 1) << 3;
    dataFrame1.id |= CanControllerRead(MCP_RXB1CTRL + 2) >> 5;
    for (uint8_t i = 0; i < 8; ++i)
    {
        dataFrame1.data[i] = CanControllerRead(MCP_RXB1CTRL + 6 + i);
    }

    // Print data from both buffers
    printf("Data from RXB0: ID = %x, Data = ", dataFrame0.id);
    for (uint8_t i = 0; i < 8; ++i)
    {
        printf("%x ", dataFrame0.data[i]);
    }
    printf("\n");

    printf("Data from RXB1: ID = %x, Data = ", dataFrame1.id);
    for (uint8_t i = 0; i < 8; ++i)
    {
        printf("%x ", dataFrame1.data[i]);
    }
    printf("\n");

    // Reset interrupt flags
    CanControllerBitModify(MCP_CANINTF, 0x00, 0x03);

    // Return data from RXB0 for compatibility with existing code
    return dataFrame0;
}
/*
struct canDataFrame_t CanReceive()
{
    // Check which buffer has data
    uint8_t canintf = CanControllerRead(MCP_CANINTF);
    struct canDataFrame_t dataFrame;
    uint8_t receiveBaseAddress;
    
    if (canintf & 0b11111100)
    {
        printf("Error: Received bad can interrupt %x\n", canintf);
        CanControllerWrite(MCP_CANINTF, 0x00);
        return;
    }
    
    else if (canintf & MCP_RX0IF)
    {
        receiveBaseAddress = MCP_RXB0CTRL;
    }
    else if (canintf & MCP_RX1IF)
    {
        receiveBaseAddress = MCP_RXB1CTRL;
    }
    
    
    dataFrame.id = 0;
    dataFrame.id |= CanControllerRead(receiveBaseAddress + 1)<<3;
    dataFrame.id |= CanControllerRead(receiveBaseAddress + 2)>>5;
    for (uint8_t i = 0; i < 8; ++i)
    {
        dataFrame.data[i] = CanControllerRead(receiveBaseAddress + 6 + i);
    }

    // Reset interrupt flags
    //should only reset the relevant flag not both
    CanControllerBitModify(MCP_CANINTF, 0x00, 0x03);

    return dataFrame;
}
*/