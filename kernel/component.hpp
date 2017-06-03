#pragma once

#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>

namespace kernel {

class kernel;

class component {
    utils::kernel_list<component>::node node_;
public:
    virtual const char *name() = 0;
    virtual void initialize() = 0;
    friend kernel;
};

} // namespace kernel

