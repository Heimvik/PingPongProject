#pragma once
#include <avr/io.h>
#include "../defines.h"

/*
To init xmem:
MCUCR:
- Set SRE in MCUCR to enable xmem
EMCUCR:
- Clear SRL2,SRL1, SRL0 to 000 to enable sigle sector wai state
- Set SRW11,SRW10 to 11 to enable longest wait state possible (better safe than sorry)
SFIOR:
- Clear XMBK to disable bus keeper
- Set XMM2 and clear XMM1 and XMM0 to enable masking og PC4-PC7
*/
void initXmem();