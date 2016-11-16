#pragma once

#include <port.h>

#define halt() \
    asm volatile("hlt")

inline void reboot() {
    unsigned char dummy = 0x02;
    while (dummy & 0x02)
        dummy = inb(0x64);
    outb(0xfe, 0x64);
    halt();
    while (1);
}

