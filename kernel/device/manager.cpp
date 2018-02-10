#include "manager.hpp"

using utils::shared_ptr;
using interfaces::block_device;
using interfaces::character_device;

namespace device {

manager::manager() {
}

void manager::initialize() {
}

void manager::register_device(shared_ptr<block_device>&) {
}

void manager::register_device(shared_ptr<character_device>& dev) {
    syslog << logger::info << "registering " << dev->name() << " device";
    const auto hash = device_hash(dev->get_type(), dev->get_id());
    character_map_.insert(utils::make_pair(hash, dev.get()));
    character_devices_.push_back(dev);
}

character_device* manager::get_character_device(const character_device::type type, const character_device::id_t id) {
    auto pair = character_map_[device_hash(type, id)];
    if (not pair) return nullptr;
    return pair->second;
}

character_device* manager::get_character_device(const char* name) {
    for (auto& dev : character_devices_) {
        if (!utils::compare(dev->name(), name)) {
            return dev;
        }
    }
    return nullptr;
}

} // namespace device

