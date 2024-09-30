#include "test.h"



void TestCan(){ 
    uint8_t* sendStr = "MESSAGE";
    CanMsg msg = (CanMsg){
        .id = 0x0,
        .length = 1,
        .byte8 = sendStr
    };

    while(1){
        can_tx(msg);
        time_spinFor(1000);
    }
}
