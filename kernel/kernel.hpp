#pragma once

#include <kernel_list.hpp>
#include "component.hpp"

namespace kernel {

class kernel {

    utils::kernel_list<component> components_;
    logger logger_;

public:

    kernel();
    void register_component(component &c);
    void run();

};

} // namespace kernel

