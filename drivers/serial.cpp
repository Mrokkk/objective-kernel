#include <shared_ptr.hpp>
#include <kernel/cpu/io.hpp>
#include <kernel/kernel.hpp>

#include "serial.hpp"
#include "tty.hpp"

namespace drivers {
namespace serial {

enum ports {
    com1 = 0x3f8,
    com2 = 0x2f8,
    com3 = 0x3e8,
    com4 = 0x2e8
};

struct driver final : tty::driver {

    explicit driver(const ports port)
            : tty::driver(tty::driver::type::serial, port_to_id(port))
            , port_(port) {
        cpu::io::outb(0x00, port + 1);    /* Disable all interrupts*/
        cpu::io::outb(0x80, port + 3);    /* Enable DLAB (set baud rate divisor) */
        cpu::io::outb(0x01, port + 0);    /* Set divisor to 3 (lo byte) 38400 baud */
        cpu::io::outb(0x00, port + 1);    /*                  (hi byte) */
        cpu::io::outb(0x03, port + 3);    /* 8 bits, no parity, one stop bit */
        cpu::io::outb(0xc7, port + 2);    /* Enable FIFO, clear them, with 14-byte threshold */
        cpu::io::outb(0x0b, port + 4);    /* IRQs enabled, RTS/DSR set */
        cpu::io::outb(0x01, port + 1);    /* Enable interrupt for receiving */
    }

    int write(const char* buffer, size_t n) {
        for (auto i = 0u; i < n; ++i) {
            send(buffer[i]);
        }
        return n;
    }

private:

    int is_transmit_empty() const {
        return cpu::io::inb(port_ + 5) & 0x20;
    }

    void send(char a) {
        if (a == '\n')
            send('\r');
        while (is_transmit_empty() == 0);
        cpu::io::outb(a, port_);
    }

    constexpr static interfaces::character_device::id_t port_to_id(const ports port) {
        switch (port) {
            case ports::com1:
                return 1;
            case ports::com2:
                return 2;
            case ports::com3:
                return 3;
            case ports::com4:
                return 4;
        }
        return 0;
    }

    const ports port_;
};

namespace {

void initialize_port(const ports port) {
    utils::shared_ptr<interfaces::character_device> ttyS = utils::make_shared<driver>(port);
    kernel::device_manager().register_device(ttyS);
}

} // namespace

void initialize() {
    initialize_port(ports::com1);
    initialize_port(ports::com2);
    initialize_port(ports::com3);
    initialize_port(ports::com4);
}

} // namespace serial
} // namespace drivers

