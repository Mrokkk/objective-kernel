#pragma once

#include <kernel_list.hpp>
#include <kernel/component.hpp>

#include "character.hpp"

namespace device {

class manager final : public kernel::component {

    utils::kernel_list<character> character_devices_;

public:

    manager() : character_devices_(&character::node_) {
    }

    const char *name() override {
        return "devices_manager";
    }

    void initialize() override {
    }

};

} // namespace device

