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
#include "console/console.hpp"
#include "logger/logger.hpp"

void initialize_drivers() {
    drivers::tty::initialize();

    auto tty2 = interfaces::device_manager::instance().get_character_device(device::character::type::tty, 2);
    if (not tty2) {
        return;
    }
    logger::set_driver(tty2);

    console::initialize(drivers::vga::print);
}

asmlinkage NORETURN void main() {
    cpu::initialize();
    memory::initialize();

    kernel::kernel kernel;

    interrupt::manager interrupt_manager;
    kernel.register_component(interrupt_manager);

    scheduler::scheduler scheduler;
    kernel.register_component(scheduler);

    device::manager device_manager;
    kernel.register_component(device_manager);

    initialize_drivers();

    kernel.run();
    while (1);
}

