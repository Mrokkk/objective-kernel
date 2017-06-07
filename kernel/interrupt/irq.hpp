#pragma once

#include <kernel/cpu/stack_frame.hpp>

namespace interrupt {

struct irq final {

    using handler = void(*)(uint32_t, cpu::stack_frame *);

    handler handler_;
    const char *name_;

};

void initialize();
void register_handler(uint32_t number, irq::handler handler, const char *name);

} // namespace interrupt

