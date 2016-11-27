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

inline void gdt_store(gdtr *gdt) {
    asm volatile(R"(
        sgdt (%%eax)
        )" :: "a" (gdt)
    );
}

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

/* DPL in flags */
#define GDT_FLAGS_RING0         (0 << 5)
#define GDT_FLAGS_RING1         (1 << 5)
#define GDT_FLAGS_RING2         (2 << 5)
#define GDT_FLAGS_RING3         (3 << 5)

/* G in flags */
#define GDT_FLAGS_4KB           (1 << 8)
#define GDT_FLAGS_1B            (0 << 8)

/* D in flags */
#define GDT_FLAGS_16BIT         (0 << 7)
#define GDT_FLAGS_32BIT         (1 << 7)

/* TYPE */
#define GDT_FLAGS_TYPE_CODE         (0x1a)
#define GDT_FLAGS_TYPE_DATA         (0x12)
#define GDT_FLAGS_TYPE_16TSS        (0x1)
#define GDT_FLAGS_TYPE_32TSS        (0x9)
#define GDT_FLAGS_TYPE_16INT_GATE   (0x6)
#define GDT_FLAGS_TYPE_32INT_GATE   (0xe)
#define GDT_FLAGS_TYPE_16TRAP_GATE  (0x7)
#define GDT_FLAGS_TYPE_32TRAP_GATE  (0xf)
#define GDT_FLAGS_TYPE_TASK_GATE    (0x5)

} // namespace gdt

} // namespace cpu

