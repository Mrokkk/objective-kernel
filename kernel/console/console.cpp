#include <cstdarg>
#include <function.hpp>
#include "console.hpp"

namespace console {

utils::function<void(const char *)> _printer;
console cout;

void initialize(printer_function func) {
    _printer = func;
    cout = func;
}

int printf(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    if (_printer)
        _printer(buf);
    return 0;
}

console &console::operator=(printer_function fn) {
    print_ = fn;
    return *this;
}

console &console::operator<<(const char *str) {
    print_(str);
    return *this;
}

console &console::operator<<(char str[]) {
    print_(str);
    return *this;
}

console &console::operator<<(int a) {
    char buf[32];
    sprintf(buf, "%d", a);
    print_(buf);
    return *this;
}

console &console::operator<<(uint32_t a) {
    char buf[32];
    sprintf(buf, "0x%08x", a);
    print_(buf);
    return *this;
}

console &console::operator<<(uint64_t a) {
    char buf[32];
    sprintf(buf, "0x%08x", a);
    print_(buf);
    return *this;
}

console &console::operator<<(uint16_t a) {
    char buf[32];
    sprintf(buf, "0x%04x", a);
    print_(buf);
    return *this;
}

console &console::operator<<(uint8_t a) {
    char buf[32];
    sprintf(buf, "0x%02x", a);
    print_(buf);
    return *this;
}

console &console::operator<<(char c) {
    char buf[3]{};
    *buf = c;
    print_(buf);
    return *this;
}

} // namespace console

