#include "character.hpp"

namespace device {

character::character(const type t, const id_t id)
        : type_(t)
        , id_(id) {
}

int character::read(char *, size_t) {
    return -1;
}

int character::write(const char *, size_t) {
    return -1;
}

} // namespace device

