#pragma once

#include <kernel/cpu/io.h>
#include <kernel/cpu/gdt.h>

__noreturn inline void reboot() {
    auto lock = cpu::irq_save();
    cpu::gdt::gdtr *gdt = nullptr;
    gdt->load();
    while (1);
}

