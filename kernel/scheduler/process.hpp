#pragma once

#include <initializer_list.hpp>
#include <list.hpp>
#include <kernel_list.hpp>
#include <shared_ptr.hpp>
#include <kernel/cpu/gdt.hpp>

namespace scheduler {

using pid_t = unsigned int;

class process {

public:

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
    utils::kernel_list<process> children;
    utils::kernel_list<process> siblings;

    process()
            : children(&process::children), siblings(&process::siblings) {
    }

};

extern utils::list<utils::shared_ptr<process>> processes;

void initialize();

} // namespace scheduler

