#include "irq.hpp"
#include "pic.hpp"
#include "stack_frame.hpp"
#include <kernel/logger/logger.hpp>

namespace cpu {

namespace irq {

namespace {

irq irqs[32];
logger log("irq");

} // namespace

asmlinkage void do_irq(uint32_t nr, stack_frame *frame) {
    if (irqs[nr].handler_) {
        irqs[nr].handler_(nr, frame);
        return;
    }
    log << logger::log_level::error << "Not handled INT " << (int)nr;
}

void register_handler(uint32_t nr, irq::handler handler, const char *name) {
    if (irqs[nr].handler_) {
        log << logger::log_level::error << "Cannot register IRQ " << nr;
        return;
    }
    irqs[nr].handler_ = handler;
    irqs[nr].name_ = name;
    pic::enable(nr);
    log << logger::log_level::info << "Registered IRQ " << nr << " " << name;
}

namespace {

void empty_isr(uint32_t, stack_frame *) {
}

} // namespace

void initialize() {
    pic::initialize();
    // Reserved IRQ handlers
    register_handler(0, &empty_isr, "timer");
    register_handler(2, &empty_isr, "cascade");
    register_handler(13, &empty_isr, "fpu");
}

} // namespace irq

} // namespace cpu

