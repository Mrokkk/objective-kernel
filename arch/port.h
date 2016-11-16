#pragma once

inline unsigned char inb(unsigned short port) {
    unsigned char rv;
    asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

inline void outb(unsigned char data, unsigned short port) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

inline void io_wait(void) {
    asm volatile (
        "jmp 1f;"
        "1:jmp 2f;"
        "2:"
    );
}

