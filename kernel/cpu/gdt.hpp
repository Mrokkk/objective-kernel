#pragma once

#include "segment.hpp"

namespace cpu {

struct tss;

namespace gdt {

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

    void load() const;

} PACKED;

void initialize();
void set_tss(tss &t);

} // namespace gdt

} // namespace cpu

