#include <shared_ptr.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/cpu/gdt.hpp>
#include "process.hpp"

namespace scheduler {

utils::list<utils::shared_ptr<process>> processes;
logger log("scheduler");
uint8_t init_process_stack[2048];
process init_process;

asmlinkage void do_scheduler() {
}

void initialize() {
    init_process.context.esp0 = reinterpret_cast<uint32_t>(init_process_stack) + 2048;
    cpu::gdt::set_tss(init_process.context);
}

} // namespace scheduler

