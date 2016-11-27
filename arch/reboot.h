#pragma once

#include <arch/port.h>
#include <arch/gdt.h>

#define halt() \
    asm volatile("hlt" ::: "memory")

inline void reboot() {
    cpu::gdt::gdtr *gdt = nullptr;
    gdt->load();
    while (1);
}

