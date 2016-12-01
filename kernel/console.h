#pragma once

#include <stdarg.h>
#include <lib/cstdio.h>

namespace console {

using printer_function = void (*)(const char *);

extern printer_function _printer;

void initialize(printer_function);
void print(const char *fmt, ...);
void print(int a);
void print(uint32_t a);
void print(uint16_t a);
void print(uint8_t a);
void print(char c);
void print(char *c);

template <typename T>
inline void print(T *a) {
    char buf[12];
    sprintf(buf, "0x%08x", reinterpret_cast<uint32_t>(a));
    _printer(buf);
}

template<typename First, typename... Rest>
inline void print(const First &first, const Rest &... rest) {
    print(first);
    print(rest...);
}

} // namespace console

