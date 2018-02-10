#include "character_device.hpp"

namespace interfaces {

character_device::character_device(const type t, const character_device::id_t id)
        : type_(t)
        , id_(id) {
}

int character_device::read(char*, size_t) {
    return -1;
}

int character_device::write(const char*, size_t) {
    return -1;
}

} // namespace interfaces

