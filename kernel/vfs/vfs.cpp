#include <list.h>
#include <shared_ptr.h>

#include "vfs.hpp"
#include "file.hpp"
#include "vnode.hpp"
#include "file_system.hpp"

namespace vfs {

utils::list<mount_point_t> mount_points;

void initialize(file_system &rootfs, block_device &bd) {
    register_device(null_bd);
    auto dev = get_device_id(bd);
    if (mount_points.size()) return;
    mount_points.push_back(utils::make_shared<mount_point>("/", rootfs, dev));
}

vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd) {
    auto dev = get_device_id(bd);
    auto dir_node = lookup(path);
    if (!dir_node) {
        return {};
    }
    auto old_fs = dir_node->fs;
    dir_node->fs = &fs;
    old_fs->sync(*dir_node);
    mount_points.push_back(utils::make_shared<mount_point>(path, fs, dev));
    return dir_node;
}

} // namespace vfs

