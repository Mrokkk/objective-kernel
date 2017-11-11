#pragma once

#include <kernel_list.hpp>

namespace device {

struct manager;

struct character {

    virtual ~character() = default;
    virtual const char *name() = 0;

    virtual int read(char *, size_t);
    virtual int write(const char *, size_t);

private:
    utils::kernel_list<character>::node node_;
    friend manager;
};

} // namespace device

