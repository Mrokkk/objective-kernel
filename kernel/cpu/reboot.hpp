#pragma once

#include <kernel/cpu/io.hpp>
#include <kernel/cpu/gdt.hpp>
#include <kernel/memory/paging/paging.hpp>

namespace cpu {

inline void reboot() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::gdtr *gdt = reinterpret_cast<cpu::gdt::gdtr *>(memory::virt_address(0u));
    gdt->load();
    while (1);
}

} // namespace cpu

