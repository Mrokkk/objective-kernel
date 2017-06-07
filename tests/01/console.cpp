#include <cstdint>
#include <cstdarg>
namespace yacppl {}
namespace utils = yacppl;
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
#include <kernel/console/console.hpp>
#include <kernel/logger/logger.hpp>
#include <unistd.h>
#include <string.hpp>

void print(const char *text) {
    auto len = yacppl::length(text);
    write(2, text, len);
}

void console_init() {
    console::initialize(&print);
    logger::set_printer_function(&print);
}

