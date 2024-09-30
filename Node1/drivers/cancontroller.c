#include "cancontroller.h"


void CanControllerInit()
{
    CanControllerReset();
    uint8_t val;
    val=CanControllerRead(MCP_CANSTAT);

    //Set 8 byte data frame
    CanControllerWrite(MCP_TXB0DLC, DATA_BYTES);
    //Turns off mask
    CanControllerBitModify(MCP_RXB0CTRL,0xF,0b01100000);
    //CanControllerWrite(MCP_CANINTE,3);
    CanControllerWrite(MCP_CANINTE, MCP_RX_INT);
    CanControlllerSetMode(MODE_LOOPBACK);
}


void CanControlllerSetMode(uint8_t mode)
{
    CanControllerWrite(MCP_CANCTRL, mode);
}


void CanControllerBitModify(uint8_t addr, uint8_t data, uint8_t mask)
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_BITMOD);
    SPITranceive(addr);
    SPITranceive(mask);
    SPITranceive(data);
    SPISetSlaveSelect(0);
}


void CanControllerRTS()
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_RTS_TX2);
    SPISetSlaveSelect(0);
}


void CanControllerReset()
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_RESET);
    SPISetSlaveSelect(0);
    _delay_ms(10);
}


void CanControllerWrite(uint8_t addr, uint8_t data)
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_WRITE);
    SPITranceive(addr);
    SPITranceive(data);
    SPISetSlaveSelect(0);
}


void CanControllerWriteMultipleBytes(uint8_t addr, uint8_t dataLength, uint8_t* data)
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_WRITE);
    SPITranceive(addr);
    for (uint8_t i = 0; i < dataLength; ++i)
    {
        SPITranceive(data[i]);
    }
    SPISetSlaveSelect(0);
}


uint8_t CanControllerRead(uint8_t addr)
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_READ);
    SPITranceive(addr);
    uint8_t data = SPITranceive(0);
    SPISetSlaveSelect(0);
    return data;
}


uint8_t CanControllerReadStatus()
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_READ_STATUS);
    uint8_t status = SPITranceive(0);
    SPISetSlaveSelect(0);
    return status;
}
