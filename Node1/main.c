#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Initialize your microcontroller here
    DDRA = 0xFF; // Set all pins of PORTA as output

    // Main loop
    while (1) {
        // Your code here
        PORTA ^= 0b1; // Toggle the first pin of PORTA
        _delay_ms(1); // Delay for 1 second

    }

    return 0; 
}