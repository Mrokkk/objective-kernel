#include <drivers/serial.h>
#include <kernel/cpu/reboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <shared_ptr.h>
#include <unique_ptr.h>
#include <kernel/console/console.hpp>

#define YATF_MAIN
#include "yatf/include/yatf.h"

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

asmlinkage __noreturn void main() {
    auto lock = cpu::make_irq_lock();
    cpu::gdt::initialize();
    cpu::idt::initialize();
    drivers::serial::initialize();
    console::initialize(drivers::serial::print);
    yatf::main(console::print);
    reboot();
    while (1);
}

