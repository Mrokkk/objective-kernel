#pragma once

#include <types.h>

namespace console {

using printer_function = void (*)(const char *);

void initialize(printer_function);
int printf(const char *fmt, ...);

class console final {

    printer_function print_ = nullptr;

public:

    console() = default;

    console &operator=(const printer_function &fn) {
        print_ = fn;
        return *this;
    }

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

    console &operator<<(const char *str) {
        print_(str);
        return *this;
    }

    console &operator<<(char str[]) {
        print_(str);
        return *this;
    }

    console &operator<<(int a) {
        char buf[32];
        sprintf(buf, "%d", a);
        print_(buf);
        return *this;
    }

    console &operator<<(uint32_t a) {
        char buf[32];
        sprintf(buf, "0x%08x", a);
        print_(buf);
        return *this;
    }

    console &operator<<(uint16_t a) {
        char buf[32];
        sprintf(buf, "0x%04x", a);
        print_(buf);
        return *this;
    }

    console &operator<<(uint8_t a) {
        char buf[32];
        sprintf(buf, "0x%02x", a);
        print_(buf);
        return *this;
    }

    console &operator<<(char c) {
        char buf[3]{};
        *buf = c;
        print_(buf);
        return *this;
    }

};

extern console cout;

} // namespace console

