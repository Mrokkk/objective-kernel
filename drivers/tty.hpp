#pragma once

#include <kernel/interfaces/character_device.hpp>

namespace drivers {
namespace tty {

struct driver : public interfaces::character_device {
    enum class type {
        console,
        serial
    };
    explicit driver(const type type, const interfaces::character_device::id_t id);
    const char* name() override;
protected:
    type type_;
    char name_[32];
};

} // namespace tty
} // namespace drivers

