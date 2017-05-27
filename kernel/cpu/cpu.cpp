#include <kernel/memory/paging/paging.hpp>
#include "gdt.hpp"
#include "idt.hpp"
#include "irq.hpp"

namespace cpu {

void reboot() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::gdtr *gdt = reinterpret_cast<cpu::gdt::gdtr *>(memory::virt_address(0u));
    gdt->load();
    while (1);
}

void initialize() {
    gdt::initialize();
    idt::initialize();
    irq::initialize();
}

} // namespace cpu
