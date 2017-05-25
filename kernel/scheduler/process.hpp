#pragma once

#include <list.hpp>
#include <shared_ptr.hpp>
#include <kernel_list.hpp>
#include <kernel/cpu/gdt.hpp>

namespace scheduler {

using pid_t = size_t;

struct process {

    enum class state {
        running,
        waiting,
        stopped,
        zombie
    };

    pid_t pid, ppid;
    cpu::gdt::tss context;
    void *stack;
    state stat = state::stopped;

};

extern utils::list<utils::shared_ptr<process>> processes;

void initialize();

} // namespace scheduler

