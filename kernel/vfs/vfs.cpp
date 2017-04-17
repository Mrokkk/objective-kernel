#include <list.h>
#include <shared_ptr.h>

#include <kernel/console/logger.hpp>

#include "vfs.hpp"
#include "file.hpp"
#include "vnode.hpp"
#include "cache.hpp"
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

bool vfs::node_exists(const utils::path &filename, const vnode_t &parent) {
    auto n = parent->fs->lookup(filename, parent);
    return static_cast<bool>(n);
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

vnode_t vfs::lookup(const path_t &path) {
    if (not mount_points_.size()) {
        warning("No root fs!");
        return {};
    }
    auto fs = mount_points_.front()->fs;
    if (cache::empty()) {
        auto root_node = fs->lookup("/");
        vnodes_.push_front(root_node);
        cache::add("/", root_node);
    }
    if (path == "/") {
        return cache::root->node;
    }
    auto path_it = path.begin() + 1; // FIXME
    vnode_t node;
    auto parent_entry = cache::root.get();
    while (path_it) {
        auto name = *path_it;
        auto child_entry = cache::find(name, parent_entry);
        if (child_entry == nullptr) {
            node = fs->lookup(utils::path(name), node);
            if (not node) {
                return {};
            }
            vnodes_.push_back(node);
            cache::add(name, node, parent_entry);
        }
        else {
            node = child_entry->node;
            if (not node) {
                warning("node is null");
            }
        }
        if (node->fs != fs) {
            if (node->fs == nullptr) {
                warning("no fs pointer");
                return {};
            }
            fs = node->fs;
        }
        ++path_it;
        parent_entry = child_entry;
    }
    return node;
}

vnode_t vfs::create(const path_t &path, vnode::type type) {
    auto dirname = utils::path(path.dirname());
    auto dir_node = lookup(dirname);
    if (not dir_node) {
        return {};
    }
    if (dir_node->node_type != vnode::type::dir) {
        return {};
    }
    utils::path filename(path.basename());
    if (node_exists(filename, dir_node)) {
        warning(to_string(type), " ", (const char *)path, " exists");
        return {};
    }
    debug("creating ", to_string(type), " ", path.get());
    auto new_node = dir_node->fs->create(filename, dir_node, type);
    if (not new_node) {
        return {};
    }
    new_node->node_type = type;
    vnodes_.push_back(new_node);
    auto cache_parent = cache::find(dir_node);
    if (not cache_parent) {
        warning("parent node for ", (const char *)path, " isn\'t cached");
        return new_node;
    }
    cache::add(filename.get(), new_node, cache_parent);
    return new_node;
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

