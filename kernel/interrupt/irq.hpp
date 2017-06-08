#pragma once

#include <kernel/cpu/stack_frame.hpp>
#include <kernel/interfaces/interrupt_handler.hpp>

namespace interrupt {

struct irq final {

    using handler = interfaces::interrupt_handler;

    handler handler_;
    const char *name_;

};

} // namespace interrupt

