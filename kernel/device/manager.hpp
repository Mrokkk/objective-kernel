#pragma once

#include <hash_map.hpp>
#include <list.hpp>
#include <kernel/interfaces/device_manager.hpp>

#include "block.hpp"
#include "character.hpp"

namespace device {

struct manager final : public interfaces::device_manager {

    explicit manager();
    void initialize() override;

    void register_device(utils::shared_ptr<block> &dev) override;
    void register_device(utils::shared_ptr<character> &dev) override;
    character *get_character_device(const character::type type, const character::id_t id) override;

private:

    constexpr uint16_t device_hash(const character::type type, const character::id_t id) const {
        return static_cast<uint16_t>(type) << 8 | static_cast<uint16_t>(id);
    }

    utils::hash_map<uint16_t, block *> block_map_;
    utils::hash_map<uint16_t, character *> character_map_;
    utils::list<utils::shared_ptr<block>> block_devices_;
    utils::list<utils::shared_ptr<character>> character_devices_;
};

} // namespace device

