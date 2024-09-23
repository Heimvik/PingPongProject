#include "spi.h"


void SPIInit()
{
/* Set MOSI and SCK output, all others input */
    set_bit(DDRB, PB5, PB7, PB4);
    //Slave select always high 
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
    printf("Waiting for SPIF\n");
    while(!(SPSR & (1<<SPIF))) {
        printf("%x\n", SPSR & (1<<SPIF));
    }

    printf("SPIF received\n");
    
    return SPDR;
}

