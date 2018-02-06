#include <shared_ptr.hpp>
#include <kernel/kernel.hpp>
#include "keyboard.hpp"
#include "serial.hpp"
#include "tty.hpp"
#include "vga.hpp"

namespace drivers {

namespace tty {

driver::driver(const driver::read_t read, const driver::write_t write,
    const device::character::id_t id)
        : device::character(device::character::type::tty, id)
        , read_(read)
        , write_(write) {
}

int driver::read(char* buffer, size_t n) {
    if (not read_) {
        return -1;
    }
    return read_(buffer, n);
}

int driver::write(const char* buffer, size_t n) {
    if (not write_) {
        return -1;
    }
    return write_(buffer, n);
}

void initialize() {
    vga::initialize();
    keyboard::initialize();
    serial::initialize();

    utils::shared_ptr<device::character> tty1 =
        utils::make_shared<drivers::tty::driver>(nullptr, vga::write, 1);
    kernel::kernel::device_manager().register_device(tty1);

    utils::shared_ptr<device::character> tty2 =
        utils::make_shared<drivers::tty::driver>(nullptr, serial::write, 2);
    kernel::kernel::device_manager().register_device(tty2);
}

} // namespace tty

} // namespace drivers
