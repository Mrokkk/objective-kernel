#pragma once

#include <kernel_list.hpp>

namespace device {

struct manager;

struct block {

    virtual ~block() = default;
    virtual const char* name() const = 0;
    virtual int read(char*, size_t);
    virtual int write(const char*, size_t);

private:
    utils::kernel_list<block>::node node_;
    friend manager;

};

} // namespace device

