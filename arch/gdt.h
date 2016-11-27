#pragma once

namespace cpu {

namespace gdt {

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __packed;

struct gdtr {
    uint16_t limit;
    uint32_t base;

    void load() {
        asm volatile(R"(
                lgdt (%%eax)
                ljmp $0x08, $1f
            1:
            )" :: "a" (this));
    }

} __packed;

#define FIRST_TSS_ENTRY 5
#define FIRST_APM_ENTRY 6

void initialize();

constexpr inline auto descriptor_selector(uint8_t num, uint32_t ring) {
    return (num << 3) | ring;
}

constexpr inline auto tss_selector(uint8_t num) {
    return (num + FIRST_TSS_ENTRY) << 3;
}

constexpr inline auto gdt_low_limit(uint32_t limit) {
    return limit & 0xffff;
}

constexpr inline auto gdt_hi_limit(uint32_t limit) {
    return (limit >> 16) & 0xf;
}

constexpr inline auto gdt_low_base(uint32_t base) {
    return base & 0xffff;
}

constexpr inline auto gdt_mid_base(uint32_t base) {
    return (base >> 16) & 0xff;
}

constexpr inline auto gdt_hi_base(uint32_t base) {
    return (base >> 24) & 0xff;
}

constexpr inline auto gdt_low_flags(uint32_t flags) {
    return (flags & 0x7f) & 0xff;
}

constexpr inline auto gdt_hi_flags(uint32_t flags) {
    return (flags >> 1) & 0xf0;
}

#define descriptor_entry(flags, base, limit) \
    { \
        gdt_low_limit(limit), \
        gdt_low_base(base), \
        gdt_mid_base(base), \
        gdt_low_flags(flags) | (1 << 7), \
        gdt_hi_limit(limit) | gdt_hi_flags(flags), \
        gdt_hi_base(base) \
    }

#define descriptor_set_base(gdt, num, base) \
    gdt[num].base_low = (base) & 0xffff; \
    gdt[num].base_middle = (((base) >> 16)) & 0xff; \
    gdt[num].base_high = ((base) >> 24) & 0xff;

#define descriptor_set_limit(gdt, num, limit) \
    gdt[num].limit_low = (limit) & 0xffff; \
    gdt[num].granularity = (((limit) >> 16) & 0xf);

#define descriptor_get_base(gdt, num) \
    (gdt[num].base_low | (gdt[num].base_middle << 16) | (gdt[num].base_high << 24))

#define descriptor_get_limit(gdt, num) \
    (gdt[num].limit_low | (gdt[num].granularity & 0xf) << 16)

#define descriptor_get_type(gdt, num) \
    (gdt[num].access & 0x1e)

namespace flags {

enum dpl {
    ring0 = 0 << 5,
    ring1 = 1 << 5,
    ring2 = 2 << 5,
    ring3 = 3 << 5
};

enum granularity {
    u4kB = 1 << 8,
    u1B = 0 << 8
};

enum size {
    u16bit = 0 << 7,
    u32bit = 1 << 7
};

enum type {
    code = 0x1a,
    data = 0x12,
    tss16 = 0x1,
    tss32 = 0x9,
    int_gate_16 = 0x6,
    int_gate_32 = 0xe,
    trap_gate_16 = 0x7,
    trap_gate_32 = 0xf,
    task_gate = 0x5
};

} // namespace flags

} // namespace gdt

} // namespace cpu

