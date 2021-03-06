#pragma once

#include <kernel_list.hpp>
#include <kernel/cpu/tss.hpp>

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
    cpu::tss context;
    void* stack;
    state stat = state::stopped;
    utils::kernel_list<process>::node running;

};

} // namespace scheduler

