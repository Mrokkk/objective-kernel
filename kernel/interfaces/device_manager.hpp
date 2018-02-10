#pragma once

#include <shared_ptr.hpp>
#include "component.hpp"
#include "block_device.hpp" // FIXME: move block and char devs here
#include "character_device.hpp"

namespace interfaces {

struct device_manager : public component {
    device_manager();
    virtual void register_device(yacppl::shared_ptr<block_device>& dev) = 0;
    virtual void register_device(yacppl::shared_ptr<character_device>& dev) = 0;
    virtual character_device* get_character_device(const character_device::type type, const character_device::id_t id) = 0;
    virtual character_device* get_character_device(const char* name) = 0;
};

} // namespace interfaces

