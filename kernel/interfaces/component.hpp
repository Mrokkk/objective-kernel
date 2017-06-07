#pragma once

#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>

namespace kernel {
class kernel;
} // namespace kernel

namespace interfaces {

struct component {
    component(const char *name);
    virtual ~component() = default;
protected:
    virtual void initialize() = 0;
    logger syslog;
private:
    utils::kernel_list<component>::node node_;
    const char *name_;

    friend kernel::kernel;
};

} // namespace kernel

