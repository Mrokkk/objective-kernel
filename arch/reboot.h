#pragma once

#include <arch/port.h>
#include <arch/gdt.h>

#define halt() \
    asm volatile("hlt" ::: "memory")

inline void reboot() {
    cpu::gdt_load(nullptr);
    while (1);
}

