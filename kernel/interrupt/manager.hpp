#pragma once

#include <kernel/interfaces/interrupt_manager.hpp>
#include "irq.hpp"

namespace interrupt {

class manager : public interfaces::interrupt_manager {

    irq irqs_[32];

    void initialize() override;

public:

    void register_handler(uint32_t nr, handler fn, const char *name) override;
    void handle_irq(uint32_t nr, cpu::stack_frame *frame) override;

};

} // namespace interrupt

