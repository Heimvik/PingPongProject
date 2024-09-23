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
    //CanControllerReset();
    //set loopback mode
    //MCP_CANCTRL = MODE_LOOPBACK;
    /*

    CanControllerWrite(MaskIdentifyer(TXBnSIDH,(uint8_t)CAN_CONTROLLER_IDENTIFIER,1);
    CanControllerWrite(MaskIdentifyer(TXBnSIDL,(uint8_t)CAN_CONTROLLER_IDENTIFIER,0);
    CanControllerWrite(TXBnDLC,DLC);
    CanControllerWrite(TXBnDm,0x8);

    CanControllerBitModify(TXBnSIDH,(uint8_t)CAN_CONTROLLER_IDENTIFIER,(uint8_t)0xF<);

    
    */
    uint8_t val;
    val=CanControllerRead(MCP_CANSTAT);
    printf("CANCTRL: %x\n", val);    
    
    CanControlllerSetMode(MODE_LOOPBACK);
    val=CanControllerRead(MCP_CANSTAT);
    printf("CANCTRL: %x\n", val);  



    

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


void CanControllerRTS(uint8_t buffer)
{
    SPISetSlaveSelect(1);
    SPITranceive(MCP_RTS_TX0+buffer);
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



/*
Request to send
Read status
Bit modify




- Set CANINTF.RXnIE is the bit that is set upon interrupt
*/
