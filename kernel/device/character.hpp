#pragma once

#include <kernel_list.hpp>

namespace device {

class manager;

struct character {

    virtual ~character() = default;
    virtual const char *name() = 0;

    virtual int read(char *, size_t) {
        return -1;
    }

    virtual int write(const char *, size_t) {
        return -1;
    }

private:
    utils::kernel_list<character>::node node_;
    friend manager;
};

} // namespace device

