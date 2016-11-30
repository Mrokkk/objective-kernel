#pragma once

namespace cpu {

namespace idt {

struct idt_entry {

    unsigned short base_lo;
    unsigned short sel;
    unsigned char  always0;
    unsigned char  flags;
    unsigned short base_hi;

    void set_gate(uint32_t base, uint16_t selector, uint32_t flags) {
        base_lo = base & 0xFFFF;
        base_hi = (base >> 16) & 0xFFFF;
        sel = selector;
        always0 = 0;
        flags = flags | 0x80;
    }

} __packed;

struct idtr {

    unsigned short limit;
    unsigned long base;

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

