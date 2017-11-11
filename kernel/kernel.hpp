#pragma once

#include <kernel_list.hpp>
#include "interfaces/component.hpp"

namespace kernel {

struct kernel {

    kernel();
    void register_component(interfaces::component &c);
    void run();

private:
    utils::kernel_list<interfaces::component> components_;
    logger logger_;
};

} // namespace kernel

