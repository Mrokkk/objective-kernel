#pragma once

#include <list.hpp>
#include <shared_ptr.hpp>
#include <kernel_list.hpp>
#include <kernel/interfaces/scheduler.hpp>
#include "process.hpp"
#include "schedulable.hpp"

namespace scheduler {

struct scheduler : public interfaces::scheduler {

    constexpr static const size_t init_stack_size_ = 2048;

    scheduler();
    void schedule() override;
    void clone_process() override;

private:
    void initialize() override;

    utils::list<utils::shared_ptr<process>> processes_;
    utils::kernel_list<process> run_queue_;
    uint8_t init_process_stack_[init_stack_size_];
    process init_process_;
    process *current_process_ = &init_process_;
    pid_t current_pid_;


};

} // namespace scheduler

