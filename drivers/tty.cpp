#include "tty.hpp"

namespace drivers {
namespace tty {

driver::driver(const type type, const interfaces::character_device::id_t id)
        : interfaces::character_device(interfaces::character_device::type::tty, id + 64 * static_cast<int>(type))
        , type_(type) {
    switch (type) {
        case type::console:
            sprintf(name_, "tty%d", id);
            break;
        case type::serial:
            sprintf(name_, "ttyS%d", id);
            break;
        default:
            break;
    }
}

const char* driver::name() {
    return name_;
}

} // namespace tty
} // namespace drivers
