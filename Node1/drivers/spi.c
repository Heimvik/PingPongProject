#include "spi.h"


void SPIInit()
{
/* Set MOSI and SCK output, all others input */
    set_bit(DDRB, PB5, PB7, PB4);
    // Why do we need this? we do not know :)
    set_bit(PORTB, PB4);
    /* Enable SPI, Master, set clock rate fck/16 */
    set_bit(SPCR, SPE, MSTR, SPR0, SPIE);
    //enable interrupt
    printf("SPI initialized\n");
}


uint8_t SPITranceive(uint8_t cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    printf("Transmitting %d\n", cData);
    while(!(SPSR & (1<<SPIF)));
    printf("Transmitted %d\n", cData);
    return SPDR;
}

void SPISetSlaveSelect(uint8_t value) {
    if (value) 
    {
        clear_bit(PORTB, PB4);
    }
    else
    {
        set_bit(PORTB, PB4);
    }
}
