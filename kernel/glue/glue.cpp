#include <kernel/interfaces/scheduler.hpp>
#include <kernel/interfaces/interrupt_manager.hpp>
#include <kernel/kernel.hpp>

asmlinkage void do_scheduler() {
    kernel::scheduler().schedule();
}

asmlinkage void do_irq(const uint32_t nr, cpu::stack_frame* frame) {
    kernel::interrupt_manager().handle_irq(nr, frame);
}

