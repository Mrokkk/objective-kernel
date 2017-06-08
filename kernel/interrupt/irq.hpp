#pragma once

#include <kernel/cpu/stack_frame.hpp>

namespace interrupt {

struct irq final {

    using handler = void(*)(uint32_t, cpu::stack_frame *);

    handler handler_;
    const char *name_;

};

} // namespace interrupt

