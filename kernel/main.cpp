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
    console::console syslog;
    syslog = drivers::serial::print;
    logger::set_console(syslog);

    time::initialize();

    scheduler::scheduler scheduler_component;
    kernel.register_component(scheduler_component);

    ramfs::ramfs ramfs;
    vfs::vfs vfs_component(ramfs);
    kernel.register_component(vfs_component);

    kernel.run();
    while (1);
}

