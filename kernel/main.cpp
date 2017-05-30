#include <drivers/vga.hpp>
#include <drivers/keyboard.hpp>

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
#include "memory/paging/paging.hpp"

asmlinkage void main() {
    memory::initialize();
    cxx::initialize();
    cpu::initialize();
    boot::initialize();
    drivers::vga::initialize();
    drivers::keyboard::initialize();
    console::initialize(drivers::vga::print);
    logger::set_console(console::cout);
    time::initialize();
    scheduler::initialize();
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    cpu::sti();
    while (1) {
        cpu::halt();
    }
}

