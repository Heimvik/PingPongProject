#ifndef DEFINES_H
#define DEFINES_H
//header guards used instead of #pragma once to avoid compiler warnings
#define F_CPU 4915200
#define BAUDRATE 9600
#define SRAM_ADDR 0x1800

#define check_bit(reg, bit) (reg & (1 << (bit)))

#define clear_bit(reg, ...) \
    do { \
        int bits[] = {__VA_ARGS__}; \
        for (int i = 0; i < sizeof(bits)/sizeof(bits[0]); ++i) { \
            reg &= ~(1 << (bits[i])); \
        } \
    } while (0)

#define set_bit(reg, ...) \
    do { \
        int bits[] = {__VA_ARGS__}; \
        for (int i = 0; i < sizeof(bits)/sizeof(bits[0]); ++i) { \
            reg |= (1 << (bits[i]));\
        } \
    } while (0)
#endif // DEFINES_H