#include <list.h>
#include <array.h>
#include <shared_ptr.h>

#include "vfs.hpp"

namespace vfs {

utils::list<mount_point *> mount_points;
null_block_device null;
utils::list<utils::shared_ptr<vnode>> vnodes;
utils::array<block_device *, 32> block_devices;
unsigned bd_index = 0;

dev_t get_device_id(block_device &bd) {
    for (auto i = 0u; i < bd_index; ++i) {
        if (block_devices[i] == &bd) {
            return i;
        }
    }
    return -1;
}

void initialize(file_system &rootfs, block_device &bd) {
    register_device(null);
    auto dev = get_device_id(bd);
    if (mount_points.size()) return;
    mount_points.push_back(new mount_point("/", rootfs, dev));
}

utils::shared_ptr<vnode> lookup(const utils::path &path) {
    return mount_points.front()->fs->lookup(path);
}

file open(const utils::path &path, file::mode mode) {
    auto node = lookup(path);
    if (node->fs == nullptr) {
        return {};
    }
    vnodes.push_back(node);
    return file(vnodes.back().get(), mode);
}

int register_device(block_device &bd) {
    block_devices[bd_index++] = &bd;
    return bd_index - 1;
}

void mount_fs(const utils::path &, file_system &) {
    // TODO
}

} // namespace vfs

