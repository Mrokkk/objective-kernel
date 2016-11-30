#include <kernel/console.h>
#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

namespace console {

printer_function _printer;

void initialize(printer_function func) {
    _printer = func;
}

void print(const char *fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    _printer(buf);
}

} // namespace console

