#include <cstdarg>
#include "console.hpp"

namespace console {

printer_function _printer = nullptr;
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

} // namespace console

