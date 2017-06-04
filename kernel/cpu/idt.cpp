#include "idt.hpp"
#include "gdt.hpp"

namespace cpu {

namespace idt {

namespace {

struct entry final {

    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;

    void set_gate(uint32_t base, uint16_t selector, uint32_t f) {
        base_lo = base & 0xFFFF;
        base_hi = (base >> 16) & 0xFFFF;
        sel = selector;
        always0 = 0;
        flags = f | 0x80;
    }

} PACKED entries[256];

struct idtr final {

    uint16_t limit;
    entry *base;

    void load() {
        asm volatile(
            "lidt (%%eax);"
            :: "a" (this)
        );
    }

} PACKED idt = {
    sizeof(entries) - 1,
    entries
};

} // namespace

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
    entries[x].set_gate(reinterpret_cast<uint32_t>(systick_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32);

#define pic_isr(x) \
    entries[x].set_gate(reinterpret_cast<uint32_t>(isr_##x), segment::kernel_cs, gdt::flags::type::trap_gate_32);

#define syscall_isr(x) \
    entries[x].set_gate(reinterpret_cast<uint32_t>(syscall_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32 | gdt::flags::ring3);

#define exception_errno(x) \
    entries[__NR_##x].set_gate(reinterpret_cast<uint32_t>(exc_##x##_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32);

#define exception_noerrno(x) \
    entries[__NR_##x].set_gate(reinterpret_cast<uint32_t>(exc_##x##_handler), segment::kernel_cs, gdt::flags::type::trap_gate_32);

void initialize() {
    #include "irqs.hpp"
    #include "exceptions.hpp"
    idt.load();
}

} // namespace idt

} // namespace cpu

