#pragma once

#include <kernel_list.hpp>
#include <kernel/interfaces/component.hpp>

#include "character.hpp"

namespace device {

class manager final : public interfaces::component {

    utils::kernel_list<character> character_devices_;

public:

    manager() : interfaces::component("device_manager"), character_devices_(&character::node_) {
    }

    void initialize() override {
    }

};

} // namespace device

