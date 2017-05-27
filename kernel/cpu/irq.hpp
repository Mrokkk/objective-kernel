#pragma once

#include "stack_frame.hpp"

namespace cpu {

namespace irq {

struct irq {

    using handler = void(*)(uint32_t, stack_frame *);

    handler handler_;
    const char *name_;

};

void initialize();
void enable(uint32_t irq);
void register_handler(uint32_t number, irq::handler handler, const char *name);

} // namespace irq

} // namespace cpu

