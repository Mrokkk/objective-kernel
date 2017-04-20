#include <cstdint>
#include <cstdarg>
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
#include <kernel/console/console.hpp>
#include <unistd.h>
#include <string.h>

void print(const char *text) {
    auto len = yacppl::length(text);
    write(2, text, len);
}

void console_init() {
    console::initialize(&print);
}

