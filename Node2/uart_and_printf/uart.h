/*
 * uart.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 * A simple interface for receiving and transmitting characters to a computer using UART via the on board USB-connector
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#define UART_RINGBUFFER_SIZE 64
/*
 * Ringbuffer for receiving characters from  
 */
typedef struct uart_ringbuffer_t
{
	uint8_t head, tail;
	char data[UART_RINGBUFFER_SIZE];
	} uart_ringbuffer;


void configure_uart(void);

int uart_getchar(uint8_t *c);
int uart_putchar(const uint8_t c);

void UART_Handler       ( void );



// Initialize. Hooks stdio functions (like `printf`)
void uart_init(uint32_t cpufreq, uint32_t baudrate);

// Send a single character
// Prefer using `printf` instead
void uart_tx(uint8_t val);

// Read a single character
// Prefer using `uart_flush` and `sscanf` instead (see below)
uint8_t uart_rx(uint8_t* val);

// Flush the internal ring buffer into your own buffer
// Example (`scanf` workaround):
//    int result;
//    char buf[80] = {0};
//
//    uart_flush(buf, sizeof(buf)/sizeof(buf[0]));
//    int numFilled = sscanf(buf, "%d", &result);
//    if(numFilled == 1){
//        printf("%d\n", result);
//    }
int uart_flush(char* buf, int len);

   

#endif /* UART_H_ */