#pragma once

#include <kernel/cpu/stack_frame.hpp>
#include <kernel/interfaces/interrupt_handler.hpp>

namespace interrupt {

struct manager;

struct irq final {

    using handler = interfaces::interrupt_handler;
    friend manager;

private:
    handler handler_;
    const char *name_;
};

} // namespace interrupt

