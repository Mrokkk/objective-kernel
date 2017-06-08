#pragma once

#include <kernel/cpu/stack_frame.hpp>

namespace interfaces {

using interrupt_handler = void(*)(uint32_t, cpu::stack_frame *);

} // namespace interfaces
