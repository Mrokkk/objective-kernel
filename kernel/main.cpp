#include <drivers/serial.hpp>
#include <drivers/vga.hpp>

#include "cpu/cpu.hpp"
#include "device/manager.hpp"
#include "interrupt/manager.hpp"
#include "kernel.hpp"
#include "logger/logger.hpp"
#include "memory/memory.hpp"
#include "scheduler/process.hpp"
#include "scheduler/scheduler.hpp"

void initialize_drivers() {
    drivers::vga::initialize();
    drivers::serial::initialize();
}

asmlinkage NORETURN void main() {
    cpu::initialize();
    memory::initialize();

    kernel kernel;

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

