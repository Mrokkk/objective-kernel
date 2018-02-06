#pragma once

#include <kernel_list.hpp>

namespace device {

struct manager;

struct character {

    using id_t = unsigned char;

    enum class type {
        none,
        tty
    };

    character(const type, const id_t);
    virtual ~character() = default;
    virtual const char* name() = 0;

    virtual int read(char*, size_t);
    virtual int write(const char*, size_t);

private:
    const type type_;
    const id_t id_;
    friend manager;
};

} // namespace device

