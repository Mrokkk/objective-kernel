#pragma once

#include <arch/port.h>

#define halt() \
    asm volatile("hlt")

static inline void gdt_load(void *gdt) {
    asm volatile(
        "    lgdt (%%eax);"
        "    ljmp $0x08, $1f;"
        "1:"
        :: "a" (gdt)
    );
}

inline void reboot() {
    gdt_load(nullptr);
    while (1);
}

