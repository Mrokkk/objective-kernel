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

null_block_device vfs::null_bd_;

vfs::vfs(file_system &rootfs, block_device &bd) {
    block_devices_[0] = &null_bd_;
    register_device(null_bd_);
    auto dev = get_device_id(bd);
    mount_points_.push_back(utils::make_shared<mount_point>("/", rootfs, dev));
}

vnode_t vfs::mount(const path_t &path, file_system &fs, block_device &bd) {
    auto dev = get_device_id(bd);
    auto dir_node = lookup(path);
    if (!dir_node) {
        return {};
    }
    auto old_fs = dir_node->fs;
    dir_node->fs = &fs;
    old_fs->sync(*dir_node);
    mount_points_.push_back(utils::make_shared<mount_point>(path, fs, dev));
    return dir_node;
}

file_t vfs::open(const path_t &path, file::mode) {
    auto node = lookup(path);
    if (not node) {
        node = create(path, vnode::type::file);
    }
    if (node->node_type != vnode::type::file) {
        return {};
    }
    return utils::make_shared<file>(node.get(), file::mode::read_write);
}

} // namespace vfs

