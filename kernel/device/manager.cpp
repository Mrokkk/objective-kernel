#include "manager.hpp"

namespace device {

manager::manager()
        : block_devices_(&block::node_)
        , character_devices_(&character::node_) {
}

void manager::initialize() {
    syslog << logger::debug << __FUNCTION__;
}

void manager::register_device(block &dev) {
    syslog << logger::info << "registering " << dev.name() << " device";
    block_devices_.push_back(dev);
}

void manager::register_device(character &dev) {
    syslog << logger::info << "registering " << dev.name() << " device";
    character_devices_.push_back(dev);
}

} // namespace device

