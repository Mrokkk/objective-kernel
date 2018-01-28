#pragma once

#include "component.hpp"
#include "../device/block.hpp" // FIXME: move block and char devs here
#include "../device/character.hpp"

namespace interfaces {

struct device_manager : public component {
    device_manager();
    static device_manager &instance();
    virtual void register_device(device::block &dev) = 0;
    virtual void register_device(device::character &dev) = 0;
private:
    static device_manager *instance_;
};

} // namespace interfaces

