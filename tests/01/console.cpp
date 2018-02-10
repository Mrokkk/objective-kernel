#include <cstdint>
#include <cstdarg>
namespace yacppl {}
namespace utils = yacppl;
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
#include <kernel/interfaces/character_device.hpp>
#include <kernel/logger/logger.hpp>
#include <unistd.h>
#include <string.hpp>


struct char_device_stub : interfaces::character_device {

    char_device_stub()
            : interfaces::character_device(interfaces::character_device::type::tty, 0) {
    }

    int write(const char *buffer, size_t n) {
        return ::write(2, buffer, n);
    }

    const char *name() {
        return "tty";
    }
};

char_device_stub char_device;

void print(const char *text) {
    auto len = yacppl::length(text);
    write(2, text, len);
}

void console_init() {
    logger::set_driver(&char_device);
}

