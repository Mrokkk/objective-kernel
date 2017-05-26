#include "gdt.hpp"
#include "idt.hpp"
#include "irq.hpp"

namespace cpu {

void initialize() {
    gdt::initialize();
    idt::initialize();
    irq::initialize();
}

} // namespace cpu
