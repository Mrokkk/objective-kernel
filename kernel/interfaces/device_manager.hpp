#pragma once

#include <shared_ptr.hpp>
#include "component.hpp"
#include "../device/block.hpp" // FIXME: move block and char devs here
#include "../device/character.hpp"

namespace interfaces {

struct device_manager : public component {
    device_manager();
    virtual void register_device(yacppl::shared_ptr<device::block>& dev) = 0;
    virtual void register_device(yacppl::shared_ptr<device::character>& dev) = 0;
    virtual device::character* get_character_device(const device::character::type type, const device::character::id_t id) = 0;
};

} // namespace interfaces

