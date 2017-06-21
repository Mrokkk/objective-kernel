#include <kernel/cpu/gdt.hpp>
#include <kernel/cpu/tss.hpp>
#include "scheduler.hpp"

namespace scheduler {

scheduler::scheduler() : run_queue_(&process::running) {
}

void scheduler::initialize() {
    init_process_.context.esp0 = reinterpret_cast<size_t>(init_process_stack_) + init_stack_size_;
    cpu::gdt::set_tss(init_process_.context);
}

void scheduler::schedule() {
    auto last = current_process_;
    if (run_queue_.empty()) {
        current_process_ = &init_process_;
    }
    cpu::context_switch(last->context, current_process_->context);
}

void scheduler::clone_process() {
    auto new_pid = ++current_pid_;
    auto new_process = utils::make_shared<process>();
    new_process->pid = new_pid;
    new_process->ppid = current_process_->pid;
    processes_.push_back(new_process);
    run_queue_.push_back(*new_process);
}

} // namespace scheduler

