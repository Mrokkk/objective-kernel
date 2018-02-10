#pragma once

#include <kernel_list.hpp>

namespace interfaces {

struct device_manager;

struct character_device {

    using id_t = uint16_t;

    enum class type {
        none,
        tty
    };

    character_device(const type, const id_t);
    virtual ~character_device() = default;
    virtual const char* name() = 0;

    virtual int read(char*, size_t);
    virtual int write(const char*, size_t);

    type get_type() const {
        return type_;
    }

    id_t get_id() const {
        return id_;
    }

private:
    const type type_;
    const id_t id_;
    friend device_manager;
};

} // namespace interfaces

