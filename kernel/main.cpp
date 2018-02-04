#include <drivers/tty.hpp>
#include <drivers/vga.hpp>

#include "cpu/cpu.hpp"
#include "device/manager.hpp"
#include "interrupt/manager.hpp"
#include "kernel.hpp"
#include "memory/memory.hpp"
#include "scheduler/process.hpp"
#include "scheduler/scheduler.hpp"
#include "vfs/file.hpp"
#include "vfs/ramfs.hpp"
#include "vfs/vfs.hpp"
#include "logger/logger.hpp"

// FIXME: initialize driver in kernel.run depending on kernel parameters
void initialize_drivers() {
    drivers::tty::initialize();

    auto tty2 = kernel::kernel::device_manager().get_character_device(device::character::type::tty, 2);
    if (not tty2) {
        return;
    }
    logger::set_driver(tty2);
}

asmlinkage NORETURN void main() {
    cpu::initialize();
    memory::initialize();

    kernel::kernel kernel;

    interrupt::manager interrupt_manager;
    kernel.register_interrupt_manager(interrupt_manager);

    scheduler::scheduler scheduler;
    kernel.register_scheduler(scheduler);

    device::manager device_manager;
    kernel.register_device_manager(device_manager);

    initialize_drivers();

    kernel.run();
    while (1);
}

