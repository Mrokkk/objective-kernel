#include <drivers/keyboard.hpp>
#include <drivers/serial.hpp>
#include <drivers/vga.hpp>

#include "boot/boot.hpp"
#include "console/console.hpp"
#include "cpu/cpu.hpp"
#include "device/manager.hpp"
#include "interrupt/manager.hpp"
#include "kernel.hpp"
#include "logger/logger.hpp"
#include "memory/memory.hpp"
#include "scheduler/process.hpp"
#include "scheduler/scheduler.hpp"
#include "time/time.hpp"
#include "vfs/file.hpp"
#include "vfs/ramfs.hpp"
#include "vfs/vfs.hpp"

void early_initialization() {
    cpu::initialize();
    boot::initialize();
    memory::initialize();
}

void initialize_drivers() {
    drivers::vga::initialize();
    drivers::serial::initialize();
    drivers::keyboard::initialize();
}

asmlinkage NORETURN void main() {
    early_initialization();

    kernel::kernel kernel;

    interrupt::manager interrupt_manager;
    kernel.register_component(interrupt_manager);

    scheduler::scheduler scheduler;
    kernel.register_component(scheduler);

    device::manager device_manager;
    kernel.register_component(device_manager);

    initialize_drivers();

    console::initialize(drivers::vga::print);
    logger::set_printer_function(drivers::serial::print);

    time::initialize();

    kernel.run();
    while (1);
}

