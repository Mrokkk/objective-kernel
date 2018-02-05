#pragma once

#include <kernel_list.hpp>
#include "interfaces/component.hpp"
#include "interfaces/device_manager.hpp"
#include "interfaces/interrupt_manager.hpp"
#include "interfaces/scheduler.hpp"

struct kernel {

    kernel();
    void register_device_manager(interfaces::device_manager& device_manager);
    void register_interrupt_manager(interfaces::interrupt_manager& interrupt_manager);
    void register_scheduler(interfaces::scheduler& scheduler);
    void run();

    static interfaces::interrupt_manager& interrupt_manager();
    static interfaces::device_manager& device_manager();
    static interfaces::scheduler& scheduler();

private:
    static interfaces::device_manager* device_manager_;
    static interfaces::interrupt_manager* interrupt_manager_;
    static interfaces::scheduler* scheduler_;
    logger logger_;
};

