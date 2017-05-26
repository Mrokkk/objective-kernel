#include "io.hpp"
#include "irq.hpp"
#include "pit.hpp"

namespace cpu {

namespace irq {

static uint16_t mask = 0xffff;

#define PIC1 0x20
#define PIC2 0xA0

#define ICW2_PIC1   0x20
#define ICW2_PIC2   0x28

using namespace io;

void irq_enable(uint32_t irq) {
    mask &= ~(1 << irq);
    auto _ = make_irq_lock();
    if (irq < 8)
        outb(mask & 0xff, PIC1 + 1);
    else
        outb(mask >> 8, PIC2 + 1);
}

void pic_disable() {
    auto _ = make_irq_lock();
    outb(0xff, PIC1 + 1);
    outb(0xff, PIC2 + 1);
}

void pic_enable() {
    auto _ = make_irq_lock();
    outb(mask & 0xff, PIC1 + 1);
    outb((mask >> 8) & 0xff, PIC2 + 1);
}

static inline void icw1_send() {
    outb(0x11, PIC1);
    outb(0x11, PIC2);
}

static inline void icw2_send() {
    outb(ICW2_PIC1, PIC1 + 1);
    outb(ICW2_PIC2, PIC2 + 1);
}

static inline void icw3_send() {
    outb(4, PIC1 + 1);
    outb(2, PIC2 + 1);
}

static inline void icw4_send() {
    outb(1, PIC1 + 1);
    outb(1, PIC2 + 1);
}

void initialize() {
    icw1_send();
    icw2_send();
    icw3_send();
    icw4_send();
    pic_disable();
}

} // namespace irq

} // namespace cpu

