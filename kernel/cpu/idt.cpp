#include <kernel/cpu/idt.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/exceptions.h>

namespace cpu {

namespace idt {

idt_entry idt_entries[256];

idtr idt = {
        sizeof(idt_entry) * 256 - 1,
        reinterpret_cast<unsigned int>(&idt_entries)
};

declare_extern_exception(divide_error)
declare_extern_exception(debug)
declare_extern_exception(nmi)
declare_extern_exception(breakpoint)
declare_extern_exception(overflow)
declare_extern_exception(bound_range)
declare_extern_exception(invalid_opcode)
declare_extern_exception(device_na)
declare_extern_exception(double_fault)
declare_extern_exception(coprocessor)
declare_extern_exception(invalid_tss)
declare_extern_exception(segment_np)
declare_extern_exception(stack_segment)
declare_extern_exception(general_protection)
declare_extern_exception(page_fault)

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint32_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = selector;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags | 0x80;
}

#define exception_initialize(x) \
    idt_set_gate(__NR_##x, reinterpret_cast<uint32_t>(exc_##x##_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32);

void initialize() {
    exception_initialize(divide_error)
    exception_initialize(debug)
    exception_initialize(nmi)
    exception_initialize(breakpoint)
    exception_initialize(overflow)
    exception_initialize(bound_range)
    exception_initialize(invalid_opcode)
    exception_initialize(device_na)
    exception_initialize(double_fault)
    exception_initialize(coprocessor)
    exception_initialize(invalid_tss)
    exception_initialize(segment_np)
    exception_initialize(stack_segment)
    exception_initialize(general_protection)
    exception_initialize(page_fault)
}

} // namespace idt

} // namespace cpu

