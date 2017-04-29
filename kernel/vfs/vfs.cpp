#include <list.h>
#include <shared_ptr.h>

#include <kernel/console/logger.hpp>

#include "vfs.hpp"
#include "file.hpp"
#include "vnode.hpp"
#include "cache.hpp"
#include "file_system.hpp"

namespace vfs {

utils::unique_ptr<vfs> vfs_;
null_block_device null_bd_;

void initialize(file_system &rootfs, block_device &bd) {
    vfs_ = new vfs(rootfs, bd);
}

vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd) {
    if (not vfs_) return {};
    return *vfs_->mount(path, fs, bd);
}

cache &get_cache() {
    return vfs_->get_cache();
}

null_block_device vfs::null_bd_;

vfs::vfs(file_system &rootfs, block_device &bd) {
    block_devices_[0] = &null_bd_;
    register_device(null_bd_); // FIXME
    auto dev = get_device_id(bd);
    mount_points_.push_back(utils::make_shared<mount_point>("/", rootfs, dev));
}

bool vfs::node_exists(const path_t &filename, const vnode_t &parent) const {
    auto n = parent->mount_point->fs->lookup(filename, parent);
    return static_cast<bool>(n);
}

maybe<vnode_t> vfs::mount(const path_t &path, file_system &fs, block_device &bd) {
    auto dev = get_device_id(bd);
    auto dir_node = lookup(path);
    if (not dir_node) {
        return dir_node;
    }
    auto m_point = utils::make_shared<mount_point>(path, fs, dev);
    mount_points_.push_back(m_point);
    dir_node->mount_point = m_point;
    return dir_node;
}

maybe<vnode_t> vfs::lookup(const path_t &path) {
    if (not mount_points_.size()) {
        warning("No root fs!");
        return error::err_no_root;
    }
    class mount_point *mount_point = mount_points_.front().get();
    auto path_it = path.begin();
    vnode_t node;
    cache::dir_entry *parent_entry = nullptr;
    while (path_it) {
        auto name = *path_it;
        auto child_entry = cache_.find(name, parent_entry);
        if (child_entry == nullptr) {
            node = mount_point->fs->lookup(utils::path(name), node);
            if (not node) {
                warning("no such file ", (const char *)path);
                return error::err_no_such_file;
            }
            node->mount_point = mount_point;
            mount_point->vnodes_.push_back(node);
            child_entry = cache_.add(name, node, parent_entry);
        }
        else {
            node = child_entry->node;
            if (not node) {
                warning("node is null");
            }
        }
        if (node->mount_point != mount_point) {
            if (node->mount_point == nullptr) {
                warning("no fs pointer for ", (const char *)path);
                return error::err_no_root;
            }
            mount_point = node->mount_point;
        }
        ++path_it;
        parent_entry = child_entry;
    }
    return node;
}

maybe<vnode_t> vfs::create(const path_t &path, vnode::type type) {
    auto dirname = utils::path(path.dirname());
    auto dir_node = lookup(dirname);
    if (not dir_node) {
        return dir_node.get_error();
    }
    if (dir_node->node_type != vnode::type::dir) {
        return error::err_no_such_file;
    }
    utils::path filename(path.basename());
    if (node_exists(filename, *dir_node)) {
        warning(to_string(type), " ", (const char *)path, " exists");
        return error::err_exists;
    }
    debug("creating ", to_string(type), " ", path.get());
    auto new_node = dir_node->mount_point->fs->create(filename, *dir_node, type);
    if (not new_node) {
        return error::err_cannot_create;
    }
    new_node->node_type = type;
    new_node->mount_point = dir_node->mount_point;
    dir_node->mount_point->vnodes_.push_back(new_node);
    auto cache_parent = cache_.find(*dir_node);
    if (not cache_parent) {
        warning("parent node for ", (const char *)path, " isn\'t cached");
        return new_node;
    }
    cache_.add(filename.get(), new_node, cache_parent);
    return new_node;
}

maybe<file_t> vfs::open(const path_t &path, file::mode mode) {
    auto node = lookup(path);
    if (not node) {
        node = create(path, vnode::type::file);
        if (not node) {
            return node.get_error();
        }
    }
    if (node->node_type != vnode::type::file) {
        return error::err_is_a_dir;
    }
    return utils::make_shared<file>(*node, mode);
}

struct cache &vfs::get_cache() {
    return cache_;
}

int vfs::register_device(block_device &bd) {
    block_devices_[bd_index_++] = &bd;
    return bd_index_ - 1;
}

dev_t vfs::get_device_id(block_device &bd) {
    for (auto i = 0u; i < bd_index_; ++i) {
        if (block_devices_[i] == &bd) {
            return i;
        }
    }
    return -1;
}

} // namespace vfs

