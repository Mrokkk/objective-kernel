#pragma once

#include <kernel_list.hpp>

namespace device {

class manager;

struct character {
    virtual const char *name() = 0;
    virtual int read(char *buffer, size_t size);
    virtual int write(const char *buffer, size_t size);
private:
    utils::kernel_list<character>::node node_;
    friend manager;
};

} // namespace device

