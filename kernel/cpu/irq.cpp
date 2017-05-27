#include "io.hpp"
#include "irq.hpp"
#include "pit.hpp"
#include "stack_frame.hpp"
#include <kernel/logger/logger.hpp>

namespace cpu {

namespace irq {

namespace {

uint16_t mask = 0xffff;
irq irqs[32];

} // namespace

#define PIC1 0x20
#define PIC2 0xA0

#define ICW2_PIC1   0x20
#define ICW2_PIC2   0x28

using namespace io;

asmlinkage void do_irq(uint32_t nr, struct stack_frame *) {
    if (irqs[nr].handler_) {
        irqs[nr].handler_(nr);
        return;
    }
    logger::get_logger() << logger::log_level::error << "Not handled INT" << nr;
}

void enable(uint32_t irq) {
    mask &= ~(1 << irq);
    auto _ = make_irq_lock();
    if (irq < 8)
        outb(mask & 0xff, PIC1 + 1);
    else
        outb(mask >> 8, PIC2 + 1);
}

void register_handler(uint32_t nr, irq::handler handler, const char *name) {
    if (irqs[nr].handler_) {
        logger::get_logger() << logger::log_level::error << "Cannot register IRQ " << nr;
        return;
    }
    irqs[nr].handler_ = handler;
    irqs[nr].name_ = name;
    enable(nr);
    logger::get_logger() << logger::log_level::info << "Registered IRQ " << nr << " " << name;
}

namespace {

void pic_disable() {
    auto _ = make_irq_lock();
    outb(0xff, PIC1 + 1);
    outb(0xff, PIC2 + 1);
}

inline void icw1_send() {
    outb(0x11, PIC1);
    outb(0x11, PIC2);
}

inline void icw2_send() {
    outb(ICW2_PIC1, PIC1 + 1);
    outb(ICW2_PIC2, PIC2 + 1);
}

inline void icw3_send() {
    outb(4, PIC1 + 1);
    outb(2, PIC2 + 1);
}

inline void icw4_send() {
    outb(1, PIC1 + 1);
    outb(1, PIC2 + 1);
}

void empty_isr(uint32_t) {
}

} // namespace

void initialize() {
    icw1_send();
    icw2_send();
    icw3_send();
    icw4_send();
    pic_disable();
    register_handler(0, &empty_isr, "timer");
    register_handler(2, &empty_isr, "cascade");
    register_handler(13, &empty_isr, "fpu");
}

} // namespace irq

} // namespace cpu

