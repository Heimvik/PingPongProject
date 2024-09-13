#include "test.h"

void TestUartTx()
{
    UartInit();
    char* testStr = "Hello";
    while (1){
        for(int i=0;i<5;i++){
            UartTransmit(testStr[i]);
        }
    }
}

void TestUartRxTx()
{
    UartInit();
    char data;
    while(1){
        data = UartReceive();
        UartTransmit(data+1);
    }
}