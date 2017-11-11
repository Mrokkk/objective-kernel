#pragma once

#include <kernel/interfaces/interrupt_manager.hpp>
#include "irq.hpp"

namespace interrupt {

struct manager : public interfaces::interrupt_manager {

    void register_handler(uint32_t nr, interfaces::interrupt_handler fn, const char *name) override;
    void handle_irq(uint32_t nr, cpu::stack_frame *frame) override;

private:
    void initialize() override;

    irq irqs_[32];
};

} // namespace interrupt

