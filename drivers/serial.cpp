#include <arch/port.h>

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

int serial_init() {

    unsigned short PORT = COM1;

    outb(0x00, PORT + 1);    /* Disable all interrupts*/
    outb(0x80, PORT + 3);    /* Enable DLAB (set baud rate divisor) */
    outb(0x01, PORT + 0);    /* Set divisor to 3 (lo byte) 38400 baud */
    outb(0x00, PORT + 1);    /*                  (hi byte) */
    outb(0x03, PORT + 3);    /* 8 bits, no parity, one stop bit */
    outb(0xc7, PORT + 2);    /* Enable FIFO, clear them, with 14-byte threshold */
    outb(0x0b, PORT + 4);    /* IRQs enabled, RTS/DSR set */

    /* Enable interrupt for receiving */
    outb(0x01, PORT + 1);

    return 0;

}

static inline int is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

void serial_send(char a, int port) {
    if (a == '\n')
        serial_send('\r', port);
    while (is_transmit_empty() == 0);
    outb(a, port);
}

void serial_print(const char *string) {
    for (; *string; string++)
        serial_send(*string, COM1);
}

