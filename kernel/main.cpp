#include <drivers/vga.h>
#include <kernel/reboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/console.h>
#include <kernel/scheduler/process.h>
#include <array.h>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/dummyfs.hpp>

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
    cpu::gdt::initialize();
    cpu::idt::initialize();
    scheduler::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    vfs::initialize();
    dummyfs::dummyfs dfs;
    auto node = dfs.lookup("dir/dummy");
    if (node.size == 0u) {
        console::print("No such file\n");
    }
    else {
        char buffer[64];
        dfs.read(node, buffer);
        console::print(reinterpret_cast<const char *>(buffer));
    }
    switch_to_user();
    while (1);
}

