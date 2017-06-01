#include <kernel/cpu/gdt.hpp>
#include "scheduler.hpp"

namespace scheduler {

scheduler *scheduler::instance_;

asmlinkage void FASTCALL(__process_switch(process *, process *next)) {
    cpu::gdt::set_tss(next->context);
}

#define process_switch(prev, next) \
    do {                                \
        asm volatile(                   \
            "push %%gs;"                \
            "pushl %%ebx;"              \
            "pushl %%ecx;"              \
            "pushl %%esi;"              \
            "pushl %%edi;"              \
            "pushl %%ebp;"              \
            "movl %%esp, %0;"           \
            "movl %2, %%esp;"           \
            "movl $1f, %1;"             \
            "pushl %3;"                 \
            "jmp __process_switch;"     \
            "1: "                       \
            "popl %%ebp;"               \
            "popl %%edi;"               \
            "popl %%esi;"               \
            "popl %%ecx;"               \
            "popl %%ebx;"               \
            "pop %%gs;"                 \
            : "=m" (prev->context.esp), \
              "=m" (prev->context.eip)  \
            : "m" (next->context.esp),  \
              "m" (next->context.eip),  \
              "a" (prev), "d" (next));  \
    } while (0)

void scheduler::schedule() {
    auto last = current_process_;
    if (run_queue_.empty()) {
        current_process_ = &init_process_;
    }
    process_switch(last, current_process_);
    ++context_switches_;
}

scheduler::scheduler() : run_queue_(&process::running) {
    instance_ = this;
}

const char *scheduler::name() {
    return name_;
}

void scheduler::initialize() {
    (void)logger_; // FIXME
    init_process_.context.esp0 = reinterpret_cast<uint32_t>(init_process_stack_) + 2048;
    cpu::gdt::set_tss(init_process_.context);
}

scheduler &scheduler::instance() {
    return *instance_;
}

asmlinkage void do_scheduler() {
    scheduler::instance().schedule();
}

} // namespace scheduler

