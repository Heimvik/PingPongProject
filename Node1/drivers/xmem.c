#include "xmem.h"


/*
To init xmem:
MCUCR:
- Set SRE in MCUCR to enable xmem
EMCUCR:
- Clear SRL2,SRL1, SRL0 to enable sigle sector wait state
- Set SRW11,SRW10 to enable longest wait state possible (better safe than sorry)
SFIOR:
- Clear XMBK to disable bus keeper
- Set XMM2 and clear XMM1 and XMM0 to enable masking og PC4-PC7
*/
void InitXmem()
{
    set_bit(MCUCR,SRE);

    clear_bit(EMCUCR,SRL2,SRL1,SRL0);
    set_bit(EMCUCR,SRW11,SRW10);

    clear_bit(SFIOR,XMBK);
    clear_bit(SFIOR,XMM0,XMM1);
    set_bit(SFIOR,XMM2);
}
