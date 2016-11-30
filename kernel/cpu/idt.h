#pragma once

namespace cpu {

namespace idt {

struct idt_entry {

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

} __packed;

struct idtr {

    uint16_t limit;
    idt_entry *base;

    void load() {
        asm volatile(
            "lidt (%%eax);"
            :: "a" (this)
        );
    }

} __packed;

void initialize();

} // namespace idt

} // namespace cpu

