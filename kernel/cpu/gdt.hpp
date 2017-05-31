#pragma once

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS 0x1b
#define USER_DS 0x23
#define FIRST_TSS 0x28

#ifndef __ASSEMBLER__

#include <algorithm.hpp>

namespace cpu {

enum segment {
    kernel_cs = KERNEL_CS,
    kernel_ds = KERNEL_DS,
    user_cs = USER_CS,
    user_ds = USER_DS,
    tss = FIRST_TSS
};

namespace gdt {

struct gdt_entry final {

    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;

    constexpr gdt_entry(uint16_t ll, uint16_t bl, uint8_t bm, uint8_t a, uint8_t g, uint8_t bh)
            : limit_low(ll), base_low(bl), base_middle(bm), access(a), granularity(g), base_high(bh) {
    }

    uint32_t limit() const {
        return limit_low | (granularity & 0xf) << 16;
    }

    void limit(uint32_t l) {
        limit_low = l & 0xffff;
        granularity = (l >> 16) & 0xf;
    }

    uint32_t base() const {
        return base_low | (base_middle << 16) | (base_high << 24);
    }

    void base(uint32_t b) {
        base_low = b & 0xffff;
        base_middle = (b >> 16) & 0xff;
        base_high = (b >> 24) & 0xff;
    }

    uint8_t type() const {
        return access & 0x1e;
    }

} PACKED;

struct gdtr final {

    uint16_t limit;
    gdt_entry *base;

    void load() const {
        asm volatile(R"(
                lgdt (%%eax)
                ljmp $0x08, $1f
            1:
        )" :: "a" (this));
    }

} PACKED;

struct tss final {

    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0 = segment::kernel_ds;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_offset = 104;
    uint8_t io_bitmap[128];

    tss() = default;

    tss(void *kernel_stack) : esp0(reinterpret_cast<uint32_t>(kernel_stack)) {
        utils::fill(io_bitmap, io_bitmap + 128, 0xff);
    }

    void load() const {
        asm volatile("ltr %%ax" :: "a" (segment::tss));
    }

} PACKED;

void initialize();

namespace detail {

constexpr inline uint16_t gdt_low_limit(uint32_t limit) {
    return limit & 0xffff;
}

constexpr inline uint32_t gdt_hi_limit(uint32_t limit) {
    return (limit >> 16) & 0xf;
}

constexpr inline uint16_t gdt_low_base(uint32_t base) {
    return base & 0xffff;
}

constexpr inline uint8_t gdt_mid_base(uint32_t base) {
    return (base >> 16) & 0xff;
}

constexpr inline uint8_t gdt_hi_base(uint32_t base) {
    return (base >> 24) & 0xff;
}

constexpr inline uint8_t gdt_low_flags(uint32_t flags) {
    return (flags & 0x7f) & 0xff;
}

constexpr inline uint8_t gdt_hi_flags(uint32_t flags) {
    return (flags >> 1) & 0xf0;
}

} // namespace detail

constexpr inline gdt_entry descriptor_entry(uint32_t flags, uint32_t base, uint32_t limit) {
    return gdt_entry(detail::gdt_low_limit(limit),
        detail::gdt_low_base(base),
        detail::gdt_mid_base(base),
        detail::gdt_low_flags(flags) | (1 << 7),
        detail::gdt_hi_limit(limit) | detail::gdt_hi_flags(flags),
        detail::gdt_hi_base(base));
}

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

extern gdt_entry gdt_entries[];

} // namespace gdt

} // namespace cpu

#endif

