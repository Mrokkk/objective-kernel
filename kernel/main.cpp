#include <array.h>

#include <drivers/vga.hpp>
#include <kernel/cpu/gdt.hpp>
#include <kernel/cpu/idt.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/cpu/reboot.hpp>
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

void write_to_file(const utils::path &path, const char *data) {
    auto file = vfs::open(path, vfs::file::mode::write);
    if (file) {
        file.write(data, utils::length(data) + 1);
    }
    else {
        console::print("Cannot open file\n");
    }
}

void read_from_file(const utils::path &path, char *data) {
    auto file = vfs::open(path);
    if (file) {
        file.read(data, 0u);
    }
    else {
        console::print("Cannot open file\n");
    }
}

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    cpu::idt::initialize();
    scheduler::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);

    auto node = vfs::create("/some_file");
    if (node->data == 0) {
        console::print("Cannot create vnode\n");
    }
    else {
        write_to_file("/some_file", "hello kernel!");
        char buffer[32];
        read_from_file("/some_file", buffer);
        console::print("File content: ", buffer, "\n");
    }

    vfs::mount_fs("/dev", ramfs);
    console::print("\nHello World!\n");
    switch_to_user();
    while (1);
}

