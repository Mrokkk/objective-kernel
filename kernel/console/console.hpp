#pragma once

#include <types.h>

namespace console {

using printer_function = void (*)(const char *);

extern printer_function _printer;

void initialize(printer_function);
int print(const char *fmt, ...);
void print(int a);
void print(uint32_t a);
void print(uint16_t a);
void print(uint8_t a);
void print(char c);
void print(char *c);

template <typename T>
inline typename utils::enable_if<
    !utils::is_same<
        typename utils::remove_const<T>::type, char
    >::value
>::type print(T *a) {
    char buf[12];
    sprintf(buf, "0x%08x", reinterpret_cast<uint32_t>(a));
    _printer(buf);
}

template<typename... Rest>
inline void print(const Rest &... rest) {
    (print(rest), ...);
}

} // namespace console

