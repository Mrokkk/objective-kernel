#include "manager.hpp"

using utils::shared_ptr;

namespace device {

manager::manager() {
}

void manager::initialize() {
}

void manager::register_device(shared_ptr<block>& dev) {
    syslog << logger::info << "registering " << dev->name() << " device";
    block_devices_.push_back(dev);
}

void manager::register_device(shared_ptr<character>& dev) {
    syslog << logger::info << "registering " << dev->name() << " device";
    const auto hash = device_hash(dev->type_, dev->id_);
    character_map_.insert(utils::make_pair(hash, dev.get()));
    character_devices_.push_back(dev);
}

character* manager::get_character_device(const character::type type, const character::id_t id) {
    auto pair = character_map_[device_hash(type, id)];
    if (not pair) return nullptr;
    return pair->second;
}

} // namespace device

