#pragma once

#include <kernel_list.hpp>
#include <kernel/interfaces/device_manager.hpp>

#include "block.hpp"
#include "character.hpp"

namespace device {

struct manager final : public interfaces::device_manager {

    explicit manager();
    void initialize() override;

    void register_device(block &dev) override;
    void register_device(character &dev) override;

private:
    utils::kernel_list<block> block_devices_;
    utils::kernel_list<character> character_devices_;
};

} // namespace device

