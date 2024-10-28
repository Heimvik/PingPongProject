/*
 * uart.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 
#include <stdint.h>

#include "sam.h"
#include "uart.h"

#define STX 0x02
#define ETX 0x03

// State machine states
typedef enum {
    WAIT_FOR_STX,
    READ_LEN_MSB,
    READ_LEN_LSB,
    READ_PAYLOAD,
    WAIT_FOR_ETX
} uart_state_t;

uart_state_t uart_state = WAIT_FOR_STX;
uint16_t payload_length = 0;
uint16_t payload_index = 0;
uint8_t payload[256]; // Adjust size as needed
//Ringbuffer for receiving multiple characters
uart_ringbuffer rx_buffer;

void handle_packet(uint8_t *payload, uint16_t length)
{
    // Process the payload here
    printf("Received valid packet with length: %d\n\r", length);
    // Example: print payload data
    for (uint16_t i = 0; i < length; i++) {
        printf("Payload[%d]: 0x%02X\n\r", i, payload[i]);
    }
}


void process_byte(uint8_t byte)
{
    switch (uart_state) {
        case WAIT_FOR_STX:
            if (byte == STX) {
                uart_state = READ_LEN_MSB;
                payload_index = 0;
                payload_length = 0;
            }
            break;
        
        case READ_LEN_MSB:
            payload_length = byte << 8; // Store MSB of the length
            uart_state = READ_LEN_LSB;
            break;
        
        case READ_LEN_LSB:
            payload_length |= byte;     // Store LSB of the length
            if (payload_length > sizeof(payload)) {
                // Invalid length, reset state
                uart_state = WAIT_FOR_STX;
                printf("ERR: Payload length too large\n\r");
            } else {
                uart_state = READ_PAYLOAD;
            }
            break;
        
        case READ_PAYLOAD:
            payload[payload_index++] = byte;
            if (payload_index >= payload_length) {
                uart_state = WAIT_FOR_ETX;
            }
            break;
        
        case WAIT_FOR_ETX:
            if (byte == ETX) {
                // Packet is valid, process the payload
                handle_packet(payload, payload_length);
            } else {
                // Packet is invalid, reset state
                printf("ERR: Invalid packet format\n\r");
            }
            uart_state = WAIT_FOR_STX;
            break;
        
        default:
            // Should never reach here, reset state machine
            uart_state = WAIT_FOR_STX;
            break;
    }
}

/**
 * \brief Handle a valid packet payload.
 *
 * \param payload The received payload data.
 * \param length The length of the payload.
 *
 * \retval void.
 */


/**
 * \brief Configure UART.
 *
 * \param void
 *
 * \retval void.
 */
void configure_uart(void)
{
	uint32_t ul_sr;

/*
Initialize UART ring buffer as empty
*/
rx_buffer.head=0;
rx_buffer.tail=0;

/*
Initialize UART communication
*/
	// Pin configuration
	// Disable interrupts on Uart receive (URXD) and transmit (UTXD) pins
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Disable the Parallel IO (PIO) of the URXD and UTXD pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Read current peripheral AB select register and set the UTXD and URXD pins to 0 (UART is connected as peripheral A)
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD) & ul_sr;

	// Enable pull up resistor on URXD and UTXD pin
	PIOA->PIO_PUER = PIO_PA8A_URXD | PIO_PA9A_UTXD;

	// Uart configuration
	
	// Enable the peripheral UART controller in Power Management Controller (PMC)
	PMC->PMC_PCER0 = 1 << ID_UART;

	// Reset and disable receiver and transmitter
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	// Set the baudrate
	UART->UART_BRGR = 547; // MCK / 16 * x = BaudRate (write x into UART_BRGR)  

	// No parity bits
	UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;	

	// Disable PDC channel
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	// Configure interrupts on receive ready and errors
	UART->UART_IDR = 0xFFFFFFFF;
	UART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME | UART_IER_PARE;

	// Enable UART interrupt in the Nested Vectored Interrupt Controller(NVIC)
	NVIC_EnableIRQ((IRQn_Type) ID_UART);

	// Enable UART receiver and transmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
	

}

/**
 * \brief Get character from UART
 *
 * \param *c location of character 
 *
 * \retval Success(0) or failure(1)
 */
int uart_getchar(uint8_t *c)
{
	// Check if a character is available in the ringbuffer
	if(rx_buffer.head == rx_buffer.tail) { //Buffer is empty
		return 1;
	}

	// Read the head character from the ringbuffer
	*c = rx_buffer.data[rx_buffer.head];
	rx_buffer.head = (rx_buffer.head + 1) % UART_RINGBUFFER_SIZE;
	return 0;
}

/*
 * \brief Sends a character through the UART interface
 *
 * \param c Character to be sent
 *
 * \retval Success(0) or failure(1).
 */
int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if((UART->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY)
	return 1;

	// Send the character
	UART->UART_THR = c;
	while(!((UART->UART_SR) & UART_SR_TXEMPTY)); // Wait for the character to be sent, can implement ring buffer to remove the wait
	return 0;
}

void UART_Handler(void)
{
	uint32_t status = UART->UART_SR;
	
	//Reset UART at overflow error and frame error
	if(status & (UART_SR_OVRE | UART_SR_FRAME | UART_SR_PARE))
	{
		UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN | UART_CR_RSTSTA;
	}
	
	//Check if message is ready to be received
	if(status & UART_SR_RXRDY)
	{
		uint8_t received_byte = UART->UART_RHR;
		//Check if receive ring buffer is full and 
		process_byte(received_byte);
		if((rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE == rx_buffer.head)
		{
			printf("ERR: UART RX buffer is full\n\r");
			rx_buffer.data[rx_buffer.tail] = UART->UART_RHR; //Throw away message
			return;
		}
		rx_buffer.data[rx_buffer.tail] = received_byte;
		rx_buffer.tail = (rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE;
	}	
}

// See https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib

extern int _end;
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
    st->st_mode = S_IFCHR;
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
        uart_putchar((uint8_t)ptr[idx]);
    }
    return len;
}

int _read(int file, char* ptr, int len){
    if(file > 1){
        return -1;
    }
    
    int nread = 0;
    for(int idx = 0; idx < len; idx++){
        int b = uart_getchar((uint8_t*)&ptr[idx]);
        nread += b;     
        if(!b){
            return nread;
        }        
    }
    return nread;
}
