# Configureation of CAN between nodes
## MCP2515
- f_osc = 16 MHz
- TQ = 2*(1+BRP)/f_osc
- BRP = 1




## SAM
- MCK = 84 MHz
- TQ = (1 + BRP)/MCK
- BRP = 20

## Other segments
- SYNC = 1
- PROP = 2  
- PHASE SEG1 = 4
- PHASE SEG2 = 4
- SMP = 0
- SJW = 1

uint32_t phase2:4;  // Phase 2 segment
uint32_t propag:4;  // Propagation time segment
uint32_t phase1:4;  // Phase 1 segment
uint32_t sjw:4;     // Synchronization jump width
uint32_t brp:8;     // Baud rate prescaler
uint32_t smp:8;     // Sampling mode


    //CanInit init = { .brp = 0x14, .phase1 = 0x2, .phase2 = 0x2, .propag = 0x1, .sjw = 0x1, .smp = 0x0 };

    //can_init(init, 0);