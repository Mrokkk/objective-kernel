#pragma once

#include <types.hpp>

namespace console {

using printer_function = void (*)(const char *);

void initialize(printer_function);
int printf(const char *fmt, ...);

class console final {

    printer_function print_ = nullptr;

public:

    console() = default;

    console &operator=(const printer_function &fn);
    console &operator<<(const char *str);
    console &operator<<(char str[]);
    console &operator<<(int a);
    console &operator<<(uint32_t a);
    console &operator<<(uint16_t a);
    console &operator<<(uint8_t a);
    console &operator<<(char c);

    template <typename T>
    typename utils::enable_if<
        !utils::is_same<
            typename utils::remove_const<T>::type, char
        >::value, console &
    >::type operator<<(T *a) {
        char buf[32];
        sprintf(buf, "0x%08x", reinterpret_cast<uint32_t>(a));
        print_(buf);
        return *this;
    }

};

extern console cout;

} // namespace console

