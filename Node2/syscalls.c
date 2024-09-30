
extern int _end;
#include "sam.h"
#include <stdio.h>
#include <sys/stat.h>

void *_sbrk(int incr){
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if(heap == NULL){
        heap = (unsigned char*)&_end;
    }
    prev_heap = heap;

    heap += incr;

    return prev_heap;
}

int _close(int file){
    return -1;
}

int _fstat(int file, struct stat* st){
    //st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file){
    return 1;
}

int _lseek(int file, int ptr, int dir){
    return 0;
}

__attribute__((naked)) void _exit(int status){
    //__asm("BKPT #0");
}

void _kill(int pid, int sig){
    return;
}

int _getpid(void){
    return -1;
}

int _write(int file, char* ptr, int len){
    if(file > 1){
        return -1;    
    }

    for(int idx = 0; idx < len; idx++){
        //uart_tx((uint8_t)ptr[idx]);
    }
    return len;
}

int _read(int file, char* ptr, int len){
    if(file > 1){
        return -1;
    }
    
    int nread = 0;
    for(int idx = 0; idx < len; idx++){
        //int b = uart_rx((uint8_t*)&ptr[idx]);
        //nread += b;     
        //if(!b){
          //  return nread;
        //}        
    }
    return nread;
}


   