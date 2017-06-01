#pragma once

#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>

namespace kernel {

class component {
public:
    utils::kernel_list<component>::node node_;
    virtual const char *name() = 0;
    virtual void initialize() = 0;
};

} // namespace kernel

