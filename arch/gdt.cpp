#include <arch/gdt.h>

namespace cpu {

namespace gdt {

gdt_entry gdt_entries[] = {

    /* Null segment */
    descriptor_entry(0, 0, 0),

    /* Kernel code segment */
    descriptor_entry(GDT_FLAGS_RING0 | GDT_FLAGS_TYPE_CODE | GDT_FLAGS_4KB | \
                     GDT_FLAGS_32BIT, 0x0, 0xffffffff),

    /* Kernel data segment */
    descriptor_entry(GDT_FLAGS_RING0 | GDT_FLAGS_TYPE_DATA | GDT_FLAGS_4KB | \
                     GDT_FLAGS_32BIT, 0x0, 0xffffffff),

    /* User code segment */
    descriptor_entry(GDT_FLAGS_RING3 | GDT_FLAGS_TYPE_CODE | GDT_FLAGS_4KB | \
                     GDT_FLAGS_32BIT, 0x0, 0xffffffff),

    /* User data segment */
    descriptor_entry(GDT_FLAGS_RING3 | GDT_FLAGS_TYPE_DATA | GDT_FLAGS_4KB | \
                     GDT_FLAGS_32BIT, 0x0, 0xffffffff),

    /* TSS */
    descriptor_entry(GDT_FLAGS_TYPE_32TSS, 0, 0)

};

gdtr gdt = {4096 * 8 - 1, reinterpret_cast<uint32_t>(&gdt_entries)};

void initialize() {
    gdt.load();
    asm volatile(R"(
        mov $0x10, %eax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
    )");
}

} // namespace gdt

} // namespace cpu

