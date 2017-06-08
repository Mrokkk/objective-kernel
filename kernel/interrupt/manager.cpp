#include <kernel/cpu/pic.hpp>
#include "manager.hpp"

namespace interrupt {

asmlinkage void do_irq(uint32_t nr, cpu::stack_frame *frame) {
    interrupt::manager::instance().handle_irq(nr, frame);
}

void manager::register_handler(uint32_t nr, handler fn, const char *name) {
    if (irqs_[nr].handler_) {
        syslog << logger::log_level::error << "Cannot register IRQ " << nr;
        return;
    }
    irqs_[nr].handler_ = fn;
    irqs_[nr].name_ = name;
    cpu::pic::enable(nr);
    syslog << logger::log_level::info << "Registered IRQ " << nr << " " << name;
}

void manager::handle_irq(uint32_t nr, cpu::stack_frame *frame) {
    if (irqs_[nr].handler_) {
        irqs_[nr].handler_(nr, frame);
        return;
    }
    syslog << logger::log_level::error << "Not handled INT " << (int)nr;
}

namespace {
void empty_isr(uint32_t, cpu::stack_frame *) {}
} // namespace

void manager::initialize() {
    cpu::pic::initialize();
    // Reserved IRQ handlers
    register_handler(0, &empty_isr, "timer");
    register_handler(2, &empty_isr, "cascade");
    register_handler(13, &empty_isr, "fpu");
}

} // namespace interrupt

