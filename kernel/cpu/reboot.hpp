#pragma once

#include <kernel/cpu/io.hpp>
#include <kernel/cpu/gdt.hpp>
#include <kernel/memory/paging.hpp>

namespace cpu {

__noreturn inline void reboot() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::gdtr *gdt = (cpu::gdt::gdtr *)(0 + KERNEL_PAGE_OFFSET);
    gdt->load();
    while (1);
}

} // namespace cpu

