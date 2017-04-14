#include <list.h>
#include <unique_ptr.h>

#include <kernel/console/logger.hpp>

#include "vfs.hpp"
#include "vnode.hpp"
#include "cache.hpp"
#include "file_system.hpp"

namespace vfs {

utils::list<vnode_t> vnodes;
extern utils::list<mount_point_t> mount_points;

const char *to_string(vnode::type type) {
    switch (type) {
        case vnode::type::file:
            return "file";
        case vnode::type::dir:
            return "dir";
        default:
            return "unknown type";
    }
}

vnode_t lookup(const path_t &path) {
    auto fs = mount_points.front()->fs;
    if (cache::empty()) {
        auto root_node = fs->lookup("");
        vnodes.push_front(root_node);
        cache::add("", root_node);
    }
    if (path == "") {
        return cache::root->node;
    }
    auto path_it = path.begin();
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
            vnodes.push_back(node);
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

bool node_exists(const utils::path &filename, const vnode_t &parent) {
    auto n = parent->fs->lookup(filename, parent);
    return (bool)n;
}

vnode_t create(const path_t &path, vnode::type type) {
    auto dir_node = lookup(utils::path(path.dirname()));
    if (not dir_node) {
        return {};
    }
    if (dir_node->node_type != vfs::vnode::type::dir) {
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
    vnodes.push_back(new_node);
    auto cache_parent = cache::find(dir_node);
    if (not cache_parent) {
        warning("parent node for ", (const char *)path, " isn\'t cached");
        return new_node;
    }
    cache::add(filename.get(), new_node, cache_parent);
    return new_node;
}

} // namespace vfs

