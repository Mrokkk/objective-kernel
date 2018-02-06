#pragma once

#include <kernel_list.hpp>
#include <kernel/logger/logger.hpp>

struct kernel;

namespace interfaces {

struct component {
    explicit component(const char* name);
    virtual ~component() = default;
protected:
    virtual void initialize() = 0;
    logger syslog;
private:
    const char* name_;
    friend kernel;
};

} // namespace kernel

