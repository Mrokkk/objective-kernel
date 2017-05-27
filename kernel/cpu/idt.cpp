#include "idt.hpp"
#include "gdt.hpp"

namespace cpu {

namespace idt {

idt_entry idt_entries[256];

idtr idt = {
        sizeof(idt_entry) * 256 - 1,
        idt_entries
};

asmlinkage void systick_handler();
asmlinkage void syscall_handler();

#define pic_isr(x) \
    asmlinkage void isr_##x();

#define exception_errno(x) \
    asmlinkage void exc_##x##_handler();

#define exception_noerrno(x) \
    asmlinkage void exc_##x##_handler();

#include "irqs.hpp"
#include "exceptions.hpp"

#undef timer_isr
#undef pic_isr
#undef syscall_isr
#undef exception_errno
#undef exception_noerrno

#define timer_isr(x) \
    idt_entries[x].set_gate(reinterpret_cast<uint32_t>(systick_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32);

#define pic_isr(x) \
    idt_entries[x].set_gate(reinterpret_cast<uint32_t>(isr_##x), segment::kernel_cs, gdt::flags::type::trap_gate_32);

#define syscall_isr(x) \
    idt_entries[x].set_gate(reinterpret_cast<uint32_t>(syscall_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32 | gdt::flags::ring3);

#define exception_errno(x) \
    idt_entries[__NR_##x].set_gate(reinterpret_cast<uint32_t>(exc_##x##_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32)

#define exception_noerrno(x) \
    idt_entries[__NR_##x].set_gate(reinterpret_cast<uint32_t>(exc_##x##_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32)

void initialize() {
    #include "irqs.hpp"
    #include "exceptions.hpp"
    idt.load();
}

} // namespace idt

} // namespace cpu

