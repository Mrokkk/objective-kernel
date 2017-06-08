#pragma once

#include <kernel/cpu/stack_frame.hpp>
#include "component.hpp"
#include "interrupt_handler.hpp"

namespace interfaces {

struct interrupt_manager : public component {
    interrupt_manager();
    static interrupt_manager &instance();
    virtual void register_handler(uint32_t nr, interrupt_handler fn, const char *name) = 0;
    virtual void handle_irq(uint32_t nr, cpu::stack_frame *frame) = 0;
private:
    static interrupt_manager *instance_;
};

} // namespace interfaces

