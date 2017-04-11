#include <list.h>
#include <array.h>
#include <shared_ptr.h>

#include "vfs.hpp"
#include "file.hpp"
#include "vnode.hpp"

namespace vfs {

utils::list<mount_point *> mount_points;
null_block_device null;
utils::list<vnode_t> vnodes;
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

file open(const path_t &path, file::mode mode) {
    auto node = lookup(path);
    if (!node) {
        return {};
    }
    vnodes.push_back(node);
    return file(vnodes.back().get(), mode);
}

int register_device(block_device &bd) {
    block_devices[bd_index++] = &bd;
    return bd_index - 1;
}

void mount_fs(const path_t &path, file_system &fs, block_device &bd) {
    // TODO
    auto dev = get_device_id(bd);
    auto dirname = path.dirname();
    auto dir_node = lookup(utils::path(dirname));
    if (!dir_node) {
        return;
    }
    dir_node->fs = &fs;
    mount_points.push_back(new mount_point(path, fs, dev));
}

} // namespace vfs

