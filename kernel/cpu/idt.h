#pragma once

namespace cpu {

namespace idt {

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char  always0;
    unsigned char  flags;
    unsigned short base_hi;
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

