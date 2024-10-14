#include "test.h"


ISR(SPI_STC_vect)
{
    printf("SPI interrupt\n");
}

ISR(TIMER1_COMPA_vect)
{
    printf("Timer interrupt\n");
}
ISR(TIMER1_COMPB_vect)
{
    printf("Timer interrupt\n");
}
ISR(TIMER1_OVF_vect)
{
    printf("Timer interrupt\n");
}
ISR(TIMER0_COMP_vect)
{
    printf("Timer interrupt\n");
}
ISR(TIMER0_OVF_vect)
{
    printf("Timer interrupt\n");
}
ISR(INT0_vect)
{
    printf("INT0 interrupt\n");
}
ISR(INT1_vect)
{
    printf("INT1 interrupt\n");
}


void TestUartTx()
{
    InitUart();
    char* testStr = "Hello";
    while (1)
    {
        for(int i=0;i<5;i++)
        {
            TransmitUart(testStr[i]);
        }
    }
}


void TestUartRxTx()
{
    InitUart();
    char data;
    while(1)
    {
        data = ReceiveUart();
        TransmitUart(data+1);
    }
}


void TestSram(void)
{
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value)
        {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        } 
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value)
        {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}


void TestADC()
{
    InitADC();
    while(1)
    {
        ReadADC();
        _delay_ms(10);
    }

}




void TestOled()
{
    // Init
    OledInit();
    OledReset();
    OledWriteOutFb();

    // Test all pixels
    
    for (int i = 0; i < 64; ++i)
    {
        for (int j = 0; j < 128; ++j)
	{
            OledSetPixel(i, j);
        }
        OledWriteOutFb();
    }
    
    // Reset
    _delay_ms(1000);
    OledReset();
    OledWriteOutFb();

    // Test text
    OledPrintLn(0, "00000000000");
    OledPrintLn(1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    OledPrintLn(2, " 2 HEIEIEIGEGIREGE");
    OledPrintLn(3, "3 Test av linja");
    OledWriteOutFb();
    _delay_ms(5000);
    OledPrintLn(5, "ASDEEFFVSW");
    OledPrintLn(2, "Ny linje 2!");
    OledWriteOutFb();
}


void TestMenu(){
    OledInit();
    OledReset();
    OledWriteOutFb();
    menuSelect();
}

void TestSPI()
{
    SPIInit();
    while(1)
    {
        SPITranceive(0xFF);
        _delay_ms(1000);
    }
}

void TestCan()
{
    SPIInit();
    CanInit();
    struct canDataFrame_t dataFrame;
    dataFrame.id = 0x01;
    dataFrame.dataLength = 8;
    for (uint8_t i = 0; i < 8; ++i)
    {
        dataFrame.data[i] = 1<<i;
    }
    printf("Sending message with id %x and data: %x %x %x %x %x %x %x %x\n", dataFrame.id, dataFrame.data[0], dataFrame.data[1], dataFrame.data[2], dataFrame.data[3], dataFrame.data[4], dataFrame.data[5], dataFrame.data[6], dataFrame.data[7]);
    printf("Configuration:\n");
    uint8_t cnf1 = CanControllerRead(MCP_CNF1);
    uint8_t cnf2 = CanControllerRead(MCP_CNF2);
    uint8_t cnf3 = CanControllerRead(MCP_CNF3);
    printf("CNF1: %x\nCNF2: %x\nCNF3: %x\n", cnf1, cnf2, cnf3);
    uint8_t canStat = CanControllerRead(MCP_CANSTAT);
    uint8_t canDlc = CanControllerRead(MCP_TXB2DLC);
    uint8_t canB2SIDH = CanControllerRead(MCP_TXB2SIDH);
    uint8_t canB2SIDL = CanControllerRead(MCP_TXB2SIDL);
    uint8_t canTxI2E = CanControllerRead(MCP_CANINTE);
    printf("CANSTAT: %x\nTXB2DLC: %x\nTXB2SIDH: %x\nTXB2SIDL: %x\nCANINTE: %x\n", canStat, canDlc, canB2SIDH, canB2SIDL, canTxI2E);
    
    uint16_t count = 0;
    while(1)
    {
        count = (count + 1) % 10000;
        if (!count) {
            CanSend(&dataFrame);
        }
        if (yesWeCanFlag)
        {
            struct canDataFrame_t dataFrame = CanReceive();
            printf("Received message with id %x and data: ", dataFrame.id);
            for (int i = 0; i < dataFrame.dataLength; ++i)
            {
                printf("%x ", dataFrame.data[i]);
            }
            printf("\n");
            return;
        }
    }
}

void TestSendJoystick()
{
    SPIInit();
    CanInit();
    struct slideOfJoy_t joystick;
    
    while(1)
    {
        joystick = ReadADC();
        CANSendJoystick(&joystick);
        _delay_ms(50);
    }
}