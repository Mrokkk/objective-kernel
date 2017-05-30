#include <array.hpp>

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

utils::array<char, 2048> user_stack;

void print_info() {
    console::cout << "Bootloader name: " << boot::bootloader_name << "\n";
    console::cout << "Boot command-line: " << boot::cmdline << "\n";
    console::cout << "Upper mem: " << (int)(boot::upper_mem / 1024) << "MiB\n";
    console::cout << "Allocator: " << (uint32_t)(memory::__end) << "\n";
    console::cout << "Page tables: " << (int)memory::paging::page_tables_number << "\n";
    for (auto i = 0u; boot::modules[i].end != 0; ++i) {
        console::cout << "Module: " << boot::modules[i].name << " @ " << boot::modules[i].start << " - " << boot::modules[i].end << "\n";
        console::cout << "Module content: " << (char *)memory::virt_address(boot::modules[0].start);
    }
    console::cout << "\nHello World!\n";
}

asmlinkage void main() {
    memory::initialize();
    cxx::initialize();
    cpu::initialize();
    drivers::vga::initialize();
    drivers::keyboard::initialize();
    console::initialize(drivers::vga::print);
    logger::set_console(console::cout);
    time::initialize();
    scheduler::initialize();
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    print_info();
    cpu::sti();
    while (1) {
        cpu::halt();
    }
}

