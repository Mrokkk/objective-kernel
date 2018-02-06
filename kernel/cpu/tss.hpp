#pragma once

#include "segment.hpp"

namespace cpu {

struct tss final {

    tss();
    tss(void* kernel_stack);
    void load() const;

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

} PACKED;

void context_switch(tss& prev, tss& next);

} // namespace cpu

