#include <kernel/console.h>
#include <stdarg.h>

namespace console {

printer_function _printer = nullptr;

void initialize(printer_function func) {
    _printer = func;
}

int print(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    if (_printer)
        _printer(buf);
    return 0;
}

void print(int a) {
    char buf[512];
    sprintf(buf, "%d", a);
    _printer(buf);
}

void print(uint32_t a) {
    char buf[12];
    sprintf(buf, "0x%08x", a);
    _printer(buf);
}

void print(uint16_t a) {
    char buf[12];
    sprintf(buf, "0x%04x", a);
    _printer(buf);
}

void print(uint8_t a) {
    char buf[12];
    sprintf(buf, "0x%02x", a);
    _printer(buf);
}

void print(char c) {
    char buf[2];
    sprintf(buf, "%c", c);
    _printer(buf);
}

void print(char *c) {
    _printer(c);
}

} // namespace console

