#include "uart.h"

/*
A lof of driver funcitonality is copied from the datasheet of the ATMEL ATmega162.
https://learn-eu-central-1-prod-fleet01-xythos.content.blackboardcdn.com/5def77a38a2f7/309080?X-Blackboard-S3-Bucket=learn-eu-central-1-prod-fleet01-xythos&X-Blackboard-Expiration=1725300000000&X-Blackboard-Signature=TSEaYFpcf%2B1dT2SyxOv9ExlJda4b%2FgYovR9JmqxZVAw%3D&X-Blackboard-Client-Id=303508&X-Blackboard-S3-Region=eu-central-1&response-cache-control=private%2C%20max-age%3D21600&response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%27Microcontroller%2520ATmega162.pdf&response-content-type=application%2Fpdf&X-Amz-Security-Token=IQoJb3JpZ2luX2VjEIz%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaDGV1LWNlbnRyYWwtMSJIMEYCIQDqUmhcE8LAijPeN73UzljvLwwKgNPCz3wjQKz5hST0ZwIhAOj%2BSsFBeZAraxvIWQ2VgIf9yTuc5wTdE4%2F3G9aweDxfKscFCKX%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEQBBoMNjM1NTY3OTI0MTgzIgz2YIIFzFSkWTp9lIwqmwV7VTYB1RBg7UzI4ZS0GJUkg7O5QJ6gQupwxd9%2Fox%2FTKR1nqVaby24IZp6%2BZUyBSTTz%2FoIkpakqnW0pY1%2BLljp1NMAAnSjQaR3uGEsGF7MC2WnD2QBZZM2vQiebEt0KXCI%2F0mCPCQRfSSICPf7gBlZ86MLgjZRUNVfdc8VHjJGF6a0fLytc6bMouYQfH5QMDEVjf7pp7HXWnpZhnnEpWSc6MY78QrhgjkKu9JWVp0LaWQFloYyvu4uEiepMg46xvVFvLQFRnfF9vq91YgjqecIOheFQ7Xk80t9mWHexzeXklkVp597Fl0ATU9TMRg6nc1BnB8myD0Kq5u06LjUGnNHOk4lnUB0hKLHog%2Bshx4XtT3MOuncjQveAJaLYivQilOFNdUUeTJzTZVOVsqz0Q0hPpHs5lb8nPRZItCtvdVDLnkFr9a95s1d0o0bXHPiygj%2F5JuDKobMM0C91haOQ7PCOVVNVw1oryiDTNyJ9NZy%2BZOiYMOOFxr9gnIvhSOfK4KkSOBQQogKNbmCNfDrA54nPkTXMegwksmJAO3TalYNxPSzmkfETpEjlOjTnb2FOa6Yyfgw0aj5SAgbH4weH9j%2BwxAXGoxa4igVb4hNQBo%2FWoX0bcZhdrYejPDO8V04vFxYU7IoYjgTFdxFcl8PqYCNoxf%2FLNFX4qSFPM1VxGaLZf3WcMkfge3NowMxbTA8gsXgcjSs2XqY5q4m2BzXfP1Q9QZAAv1R9s1sDhnQWsGIzfLBAc1K5XEm1pyCu7AxYqBUvvLTjYr8H9jnFNpVCy8vEKsvxZ%2F9JVPrXTpl%2B11b1MerNSSVId8SxvKH6IOai3TNbG4aY%2FH8yN9Wf026TQ7k8xijt6d9dfaIvQoir3g20nkMIX49exPcKjADlMLDZ1rYGOrAB3o7p0KzwB6je6jXASetyaYz7fCF7qOudUqMuczy98D4P4QRBQALS7vGFLxIzD6AEqKhhRJccsik1ETFX59W%2Bnop1wAlLznfWbhHTdIpBGUw6yXn3J3IYmQ9L7%2BPVzoQqu3c8mfm%2BYbu7zUSsU2ylf0G%2Ffsk59Q3jfU8Ebq9VfCmtyMZ4uqDqL4L6aphTLto4n5bIOdMbATQQQthwtSooXPgnYwwJ3TJM2gDv%2FfcTXxQ%3D&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20240902T120000Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=ASIAZH6WM4PL6XU5YEEX%2F20240902%2Feu-central-1%2Fs3%2Faws4_request&X-Amz-Signature=9cdbb81d8e17b710e2ff3df5bfa678d2f09046b48f774b292b18a9063e911924
*/


void UartInit( )
{   
    unsigned int ubrr = F_CPU/16/BAUDRATE-1;
    UBRR0H = (unsigned char)((ubrr & 0xFF00)>>8);
    UBRR0L = (unsigned char)(ubrr & 0xFF);

    set_bit(UCSR0B,RXEN0,TXEN0);
    // Set frame format: 8 data bits, 2 stop bit
    set_bit(UCSR0C,URSEL0,USBS0,UCSZ00,UCSZ01);
}



void UartTransmit(unsigned char data )
{
    while (!check_bit(UCSR0A,UDRE0)){}
    UDR0 = data;
}


unsigned char UartReceive()
{
    while ( !check_bit(UCSR0A,RXC0)) {}
    return UDR0;
}

int UartPutchar(char c, FILE *stream) {
    if (c == '\n') {
        UartTransmit('\r'); // Transmit carriage return before newline
    }
    UartTransmit(c);
    return 0;
}

// Wrapper function to receive a single character
int UartGetchar(FILE *stream) {
    return UartReceive();
}

void BindStdIOToUart()
{
    fdevopen(UartPutchar, UartGetchar);
}