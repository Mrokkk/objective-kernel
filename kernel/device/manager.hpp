#pragma once

#include <kernel_list.hpp>
#include <kernel/interfaces/component.hpp>

#include "character.hpp"

namespace device {

struct manager final : public interfaces::component {

    manager() : interfaces::component("device_manager"), character_devices_(&character::node_) {
    }

    void initialize() override {
    }

private:
    utils::kernel_list<character> character_devices_;
};

} // namespace device

