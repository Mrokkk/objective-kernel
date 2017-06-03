#pragma once

#include <kernel_list.hpp>

namespace scheduler {

class scheduler;

class schedulable {
    utils::kernel_list<schedulable>::node node_;
    friend scheduler;
};

} // namespace scheduler

