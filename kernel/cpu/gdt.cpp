#include "gdt.hpp"
#include "tss.hpp"

namespace cpu {

namespace gdt {

namespace {

constexpr uint16_t gdt_low_limit(uint32_t limit) {
    return limit & 0xffff;
}

constexpr uint32_t gdt_hi_limit(uint32_t limit) {
    return (limit >> 16) & 0xf;
}

constexpr uint16_t gdt_low_base(uint32_t base) {
    return base & 0xffff;
}

constexpr uint8_t gdt_mid_base(uint32_t base) {
    return (base >> 16) & 0xff;
}

constexpr uint8_t gdt_hi_base(uint32_t base) {
    return (base >> 24) & 0xff;
}

constexpr uint8_t gdt_low_flags(uint32_t flags) {
    return (flags & 0x7f) & 0xff;
}

constexpr uint8_t gdt_hi_flags(uint32_t flags) {
    return (flags >> 1) & 0xf0;
}

constexpr gdt_entry descriptor_entry(uint32_t flags, uint32_t base, uint32_t limit) {
    return gdt_entry(gdt_low_limit(limit),
        gdt_low_base(base),
        gdt_mid_base(base),
        gdt_low_flags(flags) | (1 << 7),
        gdt_hi_limit(limit) | gdt_hi_flags(flags),
        gdt_hi_base(base));
}

} // namespace

gdt_entry gdt_entries[] = {

    // Null segment
    descriptor_entry(0, 0, 0),

    // Kernel code segment
    descriptor_entry(flags::dpl::ring0 | flags::type::code | flags::granularity::u4kB | flags::size::u32bit, 0x0, 0xffffffff),

    // Kernel data segment
    descriptor_entry(flags::dpl::ring0 | flags::type::data | flags::granularity::u4kB | flags::size::u32bit, 0x0, 0xffffffff),

    // User code segment
    descriptor_entry(flags::dpl::ring3 | flags::type::code | flags::granularity::u4kB | flags::size::u32bit, 0x0, 0xffffffff),

    // User data segment
    descriptor_entry(flags::dpl::ring3 | flags::type::data | flags::granularity::u4kB | flags::size::u32bit, 0x0, 0xffffffff),

    // TSS
    descriptor_entry(flags::type::tss32, 0, sizeof(tss) - 128)

};

gdtr gdt{sizeof(gdt_entries) - 1, gdt_entries};

void gdtr::load() const {
    asm volatile(R"(
            lgdt (%%eax)
            ljmp $0x08, $1f
        1:
    )" :: "a" (this));
}

void initialize() {
    gdt.load();
    asm volatile(R"(
        mov %0, %%ds
        mov %0, %%es
        mov %0, %%fs
        mov %0, %%gs
    )" :: "r" (cpu::segment::kernel_ds));
}

void set_tss(tss &t) {
    cpu::gdt::gdt_entries[5].base(reinterpret_cast<uint32_t>(&t));
    cpu::gdt::gdt_entries[5].access &= 0xf9; /* Clear busy bit */
    t.load();
}

} // namespace gdt

} // namespace cpu

