#pragma once

#ifdef __ASSEMBLER__

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS 0x1b
#define USER_DS 0x23

#else

#include <lib/cstring.h>

namespace cpu {

enum segment {
    kernel_cs = 0x08,
    kernel_ds = 0x10,
    user_cs = 0x1b,
    user_ds = 0x23,
    tss = 0x28
};

namespace gdt {

struct gdt_entry final {

    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;

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

} __packed;

struct gdtr final {

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

struct tss final {

    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
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
    uint16_t iomap_offset;
    uint8_t io_bitmap[128];

    tss() = default;

    tss(void *kernel_stack) {
        memset(&io_bitmap, 0, 128);
        iomap_offset = 104;
        ss0 = segment::kernel_ds;
        esp0 = reinterpret_cast<uint32_t>(kernel_stack);
    }

} __packed;

void initialize();

namespace detail {

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

} // namespace detail

#define descriptor_entry(flags, base, limit) \
    { \
        detail::gdt_low_limit(limit), \
        detail::gdt_low_base(base), \
        detail::gdt_mid_base(base), \
        detail::gdt_low_flags(flags) | (1 << 7), \
        detail::gdt_hi_limit(limit) | detail::gdt_hi_flags(flags), \
        detail::gdt_hi_base(base) \
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

} // namespace gdt

} // namespace cpu

#endif

