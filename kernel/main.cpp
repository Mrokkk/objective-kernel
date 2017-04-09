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

void write_to_file(dummyfs::dummyfs &dfs, const utils::path &path, const char *data) {
    auto node = dfs.lookup(path);
    if (node.data == 0) {
        console::print("Cannot lookup for file\n");
        return;
    }
    dfs.write(node, data, utils::length(data) + 1);
}

void read_from_file(dummyfs::dummyfs &dfs, const utils::path &path, char *data) {
    auto node = dfs.lookup(path);
    if (node.data == 0) {
        console::print("Cannot lookup for file\n");
        return;
    }
    dfs.read(node, data);
}

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    cpu::idt::initialize();
    scheduler::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    vfs::initialize();
    dummyfs::dummyfs dfs;
    auto node = dfs.create("/some_file");
    if (node.data == 0) {
        console::print("Cannot create vnode\n");
    }
    else {
        write_to_file(dfs, "some_file", "hello kernel!");
        char buffer[32];
        read_from_file(dfs, "/some_file", buffer);
        console::print("File content: ");
        console::print(buffer);
    }
    console::print("\nHello World!\n");
    switch_to_user();
    while (1);
}

