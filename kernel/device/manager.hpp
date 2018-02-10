#pragma once

#include <hash_map.hpp>
#include <list.hpp>
#include <kernel/interfaces/device_manager.hpp>
#include <kernel/interfaces/block_device.hpp>
#include <kernel/interfaces/character_device.hpp>

namespace device {

struct manager final : public interfaces::device_manager {

    explicit manager();
    void initialize() override;

    void register_device(utils::shared_ptr<interfaces::block_device>& dev) override;
    void register_device(utils::shared_ptr<interfaces::character_device>& dev) override;
    interfaces::character_device* get_character_device(const interfaces::character_device::type type, const interfaces::character_device::id_t id) override;
    interfaces::character_device* get_character_device(const char* name) override;

private:

    constexpr static uint16_t device_hash(const interfaces::character_device::type type, const interfaces::character_device::id_t id) {
        return static_cast<uint16_t>(type) << 8 | static_cast<uint16_t>(id);
    }

    utils::hash_map<uint16_t, interfaces::character_device*> character_map_;
    utils::list<utils::shared_ptr<interfaces::character_device>> character_devices_;
};

} // namespace device

