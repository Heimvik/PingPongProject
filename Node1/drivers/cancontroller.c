#include "cancontroller.h"


/*
- Config mode
- Set TXBnSIDH, TXBnSIDL, TXBnDLC, TXBnDm
- Set TXBnCTRL.TXREQ
- Set CANINTF.RXnIE for enabling interrupt
- 
- Set RXB0CTRL RXM[1:0] = 0x3
*/

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
    // SJW = 0, BRP = 1
    CanControllerWrite(MCP_CNF1, 0b00000001);
    // BTLMODE = 1, SAM = 0, PHSEG1 = 1, PRSEG = 0
    CanControllerWrite(MCP_CNF2, 0b10001000);
    // SOF = 0, WAKFIL = 0, PHSEG2 = 1
    CanControllerWrite(MCP_CNF3, 0b00000001);
    CanControlllerSetMode(MODE_NORMAL);

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



