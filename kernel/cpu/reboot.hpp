#pragma once

#include <kernel/memory/paging/paging.hpp>
#include "io.hpp"
#include "gdt.hpp"

namespace cpu {

inline void reboot() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::gdtr *gdt = reinterpret_cast<cpu::gdt::gdtr *>(memory::virt_address(0u));
    gdt->load();
    while (1);
}

} // namespace cpu

