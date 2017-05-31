#include <shared_ptr.hpp>
#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/cpu/gdt.hpp>
#include "process.hpp"

namespace scheduler {

namespace {

logger log("scheduler");

} // namespace

utils::list<utils::shared_ptr<process>> processes;
utils::kernel_list<process> run_queue(&process::running);
uint8_t init_process_stack[2048];
process init_process;
process *current_process = &init_process;
size_t context_switches;

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

asmlinkage void do_scheduler() {
    auto last = current_process;
    if (run_queue.empty()) {
        current_process = &init_process;
    }
    process_switch(last, current_process);
    ++context_switches;
}

void initialize() {
    init_process.context.esp0 = reinterpret_cast<uint32_t>(init_process_stack) + 2048;
    cpu::gdt::set_tss(init_process.context);
}

} // namespace scheduler

