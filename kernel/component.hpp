#pragma once

#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>

namespace kernel {

class kernel;

struct component {
    component(const char *name);
    virtual ~component() = default;
    virtual void initialize() = 0;
    friend kernel;
private:
    utils::kernel_list<component>::node node_;
    const char *name_;
protected:
    logger syslog;
};

} // namespace kernel

