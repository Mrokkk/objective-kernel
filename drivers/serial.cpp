#include <kernel/cpu/io.h>

namespace drivers {

namespace serial {

enum ports {
    com1 = 0x3f8,
    com2 = 0x2f8,
    com3 = 0x3e8,
    com4 = 0x2e8
};

int initialize() {
    constexpr const uint16_t port = ports::com1;
    cpu::io::outb(0x00, port + 1);    /* Disable all interrupts*/
    cpu::io::outb(0x80, port + 3);    /* Enable DLAB (set baud rate divisor) */
    cpu::io::outb(0x01, port + 0);    /* Set divisor to 3 (lo byte) 38400 baud */
    cpu::io::outb(0x00, port + 1);    /*                  (hi byte) */
    cpu::io::outb(0x03, port + 3);    /* 8 bits, no parity, one stop bit */
    cpu::io::outb(0xc7, port + 2);    /* Enable FIFO, clear them, with 14-byte threshold */
    cpu::io::outb(0x0b, port + 4);    /* IRQs enabled, RTS/DSR set */
    /* Enable interrupt for receiving */
    cpu::io::outb(0x01, port + 1);
    return 0;
}

namespace {

static inline int is_transmit_empty() {
    return cpu::io::inb(ports::com1 + 5) & 0x20;
}

void send(char a, int port) {
    if (a == '\n')
        send('\r', port);
    while (is_transmit_empty() == 0);
    cpu::io::outb(a, port);
}

} // namespace anon

void print(const char *string) {
    for (; *string; string++)
        send(*string, ports::com1);
}

} // namespace serial

} // namespace drivers

