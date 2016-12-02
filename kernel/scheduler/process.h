#pragma once

#include <initializer_list.h>
#include <list.h>
#include <kernel_list.h>
#include <shared_ptr.h>
#include <kernel/cpu/gdt.h>

namespace scheduler {

class process {

public:

    enum class state {
        running,
        waiting,
        stopped,
        zombie
    };

    using pid_t = unsigned int;

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

