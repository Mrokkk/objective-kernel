#pragma once

#include <initializer_list.h>
#include <list.h>
#include <pointer.h>
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

};

extern yacppl::list<yacppl::shared_ptr<process>> processes;

void initialize();

} // namespace scheduler

