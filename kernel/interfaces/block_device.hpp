#pragma once

#include <kernel_list.hpp>

namespace interfaces {

struct manager;

struct block_device {

    virtual ~block_device() = default;
    virtual const char* name() const = 0;
    virtual int read(char*, size_t);
    virtual int write(const char*, size_t);

private:
    friend manager;
};

} // namespace interfaces

