#include "gdt.hpp"
#include "idt.hpp"

namespace cpu {

void initialize() {
    gdt::initialize();
    idt::initialize();
}

} // namespace cpu
