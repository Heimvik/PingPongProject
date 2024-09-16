#include "test.h"

void TestUartTx()
{
    InitUart();
    char* testStr = "Hello";
    while (1){
        for(int i=0;i<5;i++){
            TransmitUart(testStr[i]);
        }
    }
}

void TestUartRxTx()
{
    InitUart();
    char data;
    while(1){
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


void TestADC(){
    InitADC();
    while(1){
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
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 128; ++j) {
            OledSetPixel(i, j);
            OledWriteOutFb();
            _delay_ms(1);
        }
    }

    // Reset
    _delay_ms(1000);
    OledReset();
    OledWriteOutFb();

    // Test text
    OledPrintLn(0, "00000000000000000000");
    OledPrintLn(1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    OledPrintLn(2, " 2 HEIEIEIGEGIREGE");
    OledPrintLn(3, "3 Test av linja");
    OledWriteOutFb();
    _delay_ms(5000);
    OledPrintLn(5, "ASDEEFFVSW");
    OledPrintLn(2, "Ny linje 2!");
    OledWriteOutFb();
}
