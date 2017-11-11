#pragma once

#include <kernel_list.hpp>

namespace scheduler {

struct scheduler;

class schedulable {
    utils::kernel_list<schedulable>::node node_;
    friend scheduler;
};

} // namespace scheduler

