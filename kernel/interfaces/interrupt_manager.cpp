#include "interrupt_manager.hpp"

namespace interfaces {

interrupt_manager *interrupt_manager::instance_;

interrupt_manager::interrupt_manager() : component("interrupt_manager") {
    instance_ = this;
}

interrupt_manager &interrupt_manager::instance() {
    return *instance_;
}

} // namespace interfaces
