#include <kernel/cpu/gdt.h>
#include <lib/cstring.h>

namespace cpu {

namespace gdt {

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
    descriptor_entry(flags::type::tss32, 0, 0)

};

gdtr gdt = {4096 * 8 - 1, gdt_entries};

asmlinkage char *kernel_stack;

tss _tss(&kernel_stack);

void tss_load(unsigned short sel) {
    asm volatile(
        "ltr %%ax;"
        :: "a" (sel)
    );
}

void initialize() {
    gdt.load();
    asm volatile(R"(
        mov %0, %%ds
        mov %0, %%es
        mov %0, %%fs
        mov %0, %%gs
    )" :: "r" (cpu::segment::kernel_ds));
    gdt_entries[5].base(reinterpret_cast<uint32_t>(&_tss));
    gdt_entries[5].limit(sizeof(tss) - 128);
    tss_load(segment::tss);
}

} // namespace gdt

} // namespace cpu

