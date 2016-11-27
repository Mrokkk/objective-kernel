#pragma once

namespace cpu {

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __packed;

struct gdt {
    uint16_t limit;
    uint32_t base;
} __packed;

inline void gdt_load(gdt *gdt) {
    asm volatile(R"(
            lgdt (%%eax)
            ljmp $0x08, $1f
        1:
        )" :: "a" (gdt)
    );
}

inline void gdt_store(gdt *gdt) {
    asm volatile(R"(
        sgdt (%%eax)
        )" :: "a" (gdt)
    );
}

} // namespace cpu

