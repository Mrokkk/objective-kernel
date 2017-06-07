#pragma once

#include <list.hpp>
#include <shared_ptr.hpp>
#include <kernel_list.hpp>
#include <kernel/component.hpp>
#include "process.hpp"
#include "schedulable.hpp"

namespace scheduler {

class scheduler : public kernel::component {

    constexpr static const char *name_ = "scheduler";
    constexpr static const size_t init_stack_size_ = 2048;
    static scheduler *instance_;

    utils::list<utils::shared_ptr<process>> processes_;
    utils::kernel_list<process> run_queue_;
    uint8_t init_process_stack_[init_stack_size_];
    process init_process_;
    process *current_process_ = &init_process_;
    pid_t current_pid_;

public:

    scheduler();
    void initialize() override;
    void schedule();
    void clone_process();
    static scheduler &instance();

};

} // namespace scheduler

