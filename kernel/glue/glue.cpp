#include <kernel/interfaces/scheduler.hpp>
#include <kernel/interfaces/interrupt_manager.hpp>

asmlinkage void do_scheduler() {
    interfaces::scheduler::instance().schedule();
}

asmlinkage void do_irq(uint32_t nr, cpu::stack_frame *frame) {
    interfaces::interrupt_manager::instance().handle_irq(nr, frame);
}

