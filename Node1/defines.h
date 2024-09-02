#define F_CPU 4915200
#define BAUDRATE 9600
#define set_bit(reg, bit) (reg |= (1 << (bit)))
#define clear_bit(reg, bit) (reg &= ~(1 << (bit)))
#define test_bit(reg, bit) (reg & (1 << (bit)))
