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
    static scheduler *instance_;

    utils::list<utils::shared_ptr<process>> processes_;
    utils::kernel_list<process> run_queue_;
    uint8_t init_process_stack_[2048];
    process init_process_;
    process *current_process_ = &init_process_;
    size_t context_switches_;
    logger logger_;

public:

    scheduler();
    const char *name() override;
    void initialize() override;
    void schedule();
    static scheduler &instance();

};

} // namespace scheduler

