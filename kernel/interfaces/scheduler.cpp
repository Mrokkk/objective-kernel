#include "scheduler.hpp"

namespace interfaces {

scheduler *scheduler::instance_;

scheduler::scheduler() : component("scheduler") {
    instance_ = this;
}

scheduler &scheduler::instance() {
    return *instance_;
}

} // namespace interfaces

