#include <shared_ptr.h>
#include <unique_ptr.h>

#include <drivers/serial.hpp>
#include <kernel/cpu/gdt.hpp>
#include <kernel/cpu/idt.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/cpu/reboot.hpp>
#include <kernel/console/console.hpp>

#define YATF_MAIN
#include "yatf/include/yatf.h"

namespace {

void write_to_file(const utils::path &path, const char *data) {
    auto file = vfs::open(path, vfs::file::mode::write);
    REQUIRE(file);
    file.write(data, utils::length(data) + 1);
}

void read_from_file(const utils::path &path, char *data) {
    auto file = vfs::open(path);
    REQUIRE(file);
    file.read(data, 0u);
}

} // namespace

namespace test_cases {

TEST(kernel_allocator, can_allocate_and_free) {
    for (int i = 0; i < 1024; i++) {
        auto a = yacppl::make_shared<int>(1);
        REQUIRE(*a == 1);
        auto b = yacppl::make_unique<char>(2);
        REQUIRE(*b == 2);
        REQUIRE(reinterpret_cast<unsigned int>(b.get()) != reinterpret_cast<unsigned int>(a.get()));
        auto c = new int{498};
        REQUIRE(*c == 498);
        REQUIRE(reinterpret_cast<unsigned int>(c) != reinterpret_cast<unsigned int>(a.get()));
        REQUIRE(reinterpret_cast<unsigned int>(c) != reinterpret_cast<unsigned int>(b.get()));
        REQUIRE(*a == 1);
        REQUIRE(*b == 2);
        delete c;
        auto d = new int(5);
        delete d;
        REQUIRE(c == d);
        REQUIRE(reinterpret_cast<unsigned int>(c) > reinterpret_cast<unsigned int>(b.get()));
        REQUIRE(reinterpret_cast<unsigned int>(b.get()) > reinterpret_cast<unsigned int>(a.get()));
    }
}

TEST(vfs, can_do_things) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    auto node = vfs::create("/some_file");
    REQUIRE(node);
    REQUIRE_EQ(node->fs, &ramfs);
    write_to_file("/some_file", "hello kernel!");
    char buffer[32];
    utils::fill(buffer, 32, 0);
    read_from_file("/some_file", buffer);
    REQUIRE_EQ((const char *)buffer, "hello kernel!");
    auto dir_node = vfs::create("/some_dir", vfs::vnode::type::dir);
    REQUIRE(dir_node);
    auto node2 = vfs::create("/some_dir/file");
    REQUIRE(node2);
    write_to_file("/some_dir/file", "hello world from file in dir!");
    utils::fill(buffer, 32, 0);
    read_from_file("/some_dir/file", buffer);
    REQUIRE_EQ((const char *)buffer, "hello world from file in dir!");
    ramfs::ramfs ramfs2;
    auto dev_node = vfs::create("/dev", vfs::vnode::type::dir);
    auto mounted_node = vfs::mount_fs("/dev", ramfs2);
    REQUIRE(mounted_node);
    REQUIRE_EQ(mounted_node->fs, &ramfs2);
    dev_node = vfs::lookup("/dev");
    REQUIRE(dev_node);
    REQUIRE_EQ(dev_node->fs, &ramfs2);
    auto node3 = vfs::create("/dev/file");
    REQUIRE(node3);
    REQUIRE_EQ(node3->fs, &ramfs2);
    REQUIRE_EQ((const char *)node3->fs->name(), "ramfs");
    write_to_file("/dev/file", "asdfg\n");
    utils::fill(buffer, 32, 0);
    read_from_file("/dev/file", buffer);
    REQUIRE_EQ((const char *)buffer, "asdfg\n");
}

} // namespace test_cases

extern char cmdline[];
extern char *cmdline_ptr;

asmlinkage __noreturn void main() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::initialize();
    cpu::idt::initialize();
    drivers::serial::initialize();
    console::initialize(drivers::serial::print);
    yatf::config config{true, false, false};
    console::print("Cmdline: ", cmdline, "\n");
    yatf::run_one(console::print, cmdline, config);
    reboot();
    while (1);
}

