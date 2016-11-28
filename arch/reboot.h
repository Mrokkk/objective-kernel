#pragma once

#include <arch/port.h>
#include <arch/gdt.h>

__noreturn inline void reboot() {
    auto lock = cpu::irq_save();
    cpu::gdt::gdtr *gdt = nullptr;
    gdt->load();
    while (1);
}

