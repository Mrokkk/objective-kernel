#pragma once

#include "component.hpp"

namespace interfaces {

struct scheduler : public component {
    scheduler();
    virtual void schedule() = 0;
    virtual void clone_process() = 0;
};

} // namespace interfaces

