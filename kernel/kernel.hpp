#pragma once

#include <kernel_list.hpp>
#include "interfaces/component.hpp"

namespace kernel {

class kernel {

    utils::kernel_list<interfaces::component> components_;
    logger logger_;

public:

    kernel();
    void register_component(interfaces::component &c);
    void run();

};

} // namespace kernel

