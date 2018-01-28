#include "device_manager.hpp"

namespace interfaces {

device_manager *device_manager::instance_;

device_manager::device_manager() : component("device_manager") {
    instance_ = this;
}

device_manager &device_manager::instance() {
    return *instance_;
}

} // namespace interfaces
