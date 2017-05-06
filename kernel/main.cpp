#include <array.h>

#include <drivers/vga.hpp>
#include <kernel/boot.hpp>
#include <kernel/cpu/gdt.hpp>
#include <kernel/cpu/idt.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/cpu/reboot.hpp>
#include <kernel/cpp_support.hpp>
#include <kernel/memory/memory.hpp>
#include <kernel/console/logger.hpp>
#include <kernel/console/console.hpp>
#include <kernel/scheduler/process.hpp>

utils::array<char, 2048> user_stack;

#define switch_to_user() \
    asm volatile(                           \
        "pushl %0;"                         \
        "pushl %2;"                         \
        "pushl $0x0;"                       \
        "pushl %1;"                         \
        "push $1f;"                         \
        "iret;"                             \
        "1:"                                \
        "mov %0, %%eax;"                    \
        "mov %%ax, %%ds;"                   \
        "mov %%ax, %%es;"                   \
        "mov %%ax, %%fs;"                   \
        "mov %%ax, %%gs;"                   \
        :: "i" (cpu::segment::user_ds),     \
           "i" (cpu::segment::user_cs),     \
           "r" (&user_stack[2048])          \
    )

asmlinkage __noreturn void main() {
    memory::initialize();
    cpp_support::initialize();
    cpu::gdt::initialize();
    cpu::idt::initialize();
    scheduler::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    console::print("Boot command-line: ", boot::cmdline, "\n");
    console::print("Upper mem: ", (int)(boot::upper_mem / 1024), "MiB\n");
    console::print("Allocator: ", (uint32_t)(memory::phys_address(memory::allocator_memory)), "\n");
    console::print("Page tables: ", (int)memory::paging::page_tables_number, "\n");
    console::print("\nHello World!\n");
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    switch_to_user();
    while (1);
}

