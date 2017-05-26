#include <array.hpp>

#include <drivers/vga.hpp>
#include <kernel/cpu/cpu.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/boot/boot.hpp>
#include <kernel/cpu/reboot.hpp>
#include <kernel/cpp_support.hpp>
#include <kernel/memory/memory.hpp>
#include <kernel/console/logger.hpp>
#include <kernel/console/console.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/scheduler/process.hpp>

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
    cpp_support::initialize();
    cpu::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    logger::set_console(console::cout);
    scheduler::initialize();
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    print_info();
    asm volatile(R"(
        pushl %0
        pushl %2
        pushl $0x0
        pushl %1
        push $1f
        iret
        1:
        mov %0, %%ds
        mov %0, %%es
        mov %0, %%fs
        mov %0, %%gs)"
        :: "a" (cpu::segment::user_ds),
           "i" (cpu::segment::user_cs),
           "r" (&user_stack[2048])
        : "memory"
    );
    while (1);
}

