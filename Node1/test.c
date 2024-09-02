#include "test.h"

void UartTx()
{
    UartInit();
    char* testStr = "Hello";
    while (1){
        for(int i=0;i<5;i++){
            UartTransmit(testStr[i]);
        }
    }
}

void UartRxTx()
{
    UartInit();
    char data;
    while(1){
        data = UartReceive();
        UartTransmit(data+1);
    }
}


void LatchTest()
{
    UartInit();
    BindStdIOToUart();
    set_bit(MCUCR, SRE);
    set_bit(SFIOR,XMM2);
    volatile uint8_t *address = (volatile uint8_t *)0x1000; // Create a pointer to the memory address
    volatile uint8_t *address2 = (volatile uint8_t *)0x1F05;
    while (1)
    {
        printf("Address: %p\n", address);
        *address = 0xFF; // Set the value at the memory address to 0xFF
        _delay_ms(3000);
        printf("Address: %p\n", address2);
        *address2 = 0xFF; // Set the value at the memory address to 0x00
        _delay_ms(2000);
    }


}


void SramTest(void)
    {
        set_bit(MCUCR, SRE);
        //set_bit(SFIOR,XMM2);
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
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            ext_ram[i] = some_value;
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                write_errors++;
            }
        }
        // Retrieval phase: Check that no values were changed during or after the write phase
        srand(seed);
        // reset the PRNG to the state it had before the write phase
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                retrieval_errors++;
            }
        }
        printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}




