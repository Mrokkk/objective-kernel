#include <shared_ptr.hpp>
#include <unique_ptr.hpp>

#include <drivers/serial.hpp>
#include <drivers/tty.hpp>
#include <kernel/boot/boot.hpp>
#include <kernel/cpu/cpu.hpp>
#include <kernel/device/manager.hpp>
#include <kernel/interfaces/device_manager.hpp>
#include <kernel/interrupt/manager.hpp>
#include <kernel/kernel.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/memory/memory.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/vfs.hpp>

#include <yatf.hpp>

namespace {

//void write_to_file(const utils::path &path, const char *data) {
    //auto file = vfs::open(path, vfs::file::mode::write);
    //REQUIRE(file);
    //file->write(data, utils::length(data) + 1);
//}

//void read_from_file(const utils::path &path, char *data) {
    //auto file = vfs::open(path);
    //REQUIRE(file);
    //// FIXME
    //file->read(data, 32);
//}

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

} // namespace test_cases

asmlinkage void main() {
    cpu::initialize();
    memory::initialize();

    kernel kernel;

    interrupt::manager interrupt_manager;
    kernel.register_interrupt_manager(interrupt_manager);

    device::manager device_manager;
    kernel.register_device_manager(device_manager);

    drivers::serial::initialize();

    auto ttyS = device_manager.get_character_device("ttyS1");
    if (not ttyS) {
        return;
    }
    logger::set_driver(ttyS);

    yatf::config config{true, false, false};
    yatf::run_one(logger::printf, boot::boot_data.cmdline, config);
    cpu::reboot();
    while (1);
}

