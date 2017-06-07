#include <drivers/vga.hpp>
#include <drivers/serial.hpp>
#include <drivers/keyboard.hpp>

#include "kernel.hpp"
#include "cxx.hpp"
#include "cpu/cpu.hpp"
#include "vfs/vfs.hpp"
#include "vfs/file.hpp"
#include "time/time.hpp"
#include "vfs/ramfs.hpp"
#include "boot/boot.hpp"
#include "memory/memory.hpp"
#include "logger/logger.hpp"
#include "device/manager.hpp"
#include "console/console.hpp"
#include "scheduler/process.hpp"
#include "scheduler/scheduler.hpp"
#include "memory/paging/paging.hpp"

asmlinkage NORETURN void main() {
    memory::initialize();
    cxx::initialize();
    cpu::initialize();
    boot::initialize();

    kernel::kernel kernel;

    drivers::vga::initialize();
    drivers::serial::initialize();
    drivers::keyboard::initialize();

    console::initialize(drivers::vga::print);
    logger::set_printer_function(drivers::serial::print);

    time::initialize();

    scheduler::scheduler scheduler;
    kernel.register_component(scheduler);

    device::manager device_manager;
    kernel.register_component(device_manager);

    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    kernel.register_component(vfs);

    kernel.run();
    while (1);
}

