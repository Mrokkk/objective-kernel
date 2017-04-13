#include <list.h>

#include "vfs.hpp"
#include "vnode.hpp"
#include "dir_entry.hpp"
#include "file_system.hpp"

#include <kernel/console/logger.hpp>

namespace vfs {

utils::list<vnode_t> vnodes;
extern utils::list<mount_point *> mount_points;

namespace cache {

dir_entry *root = nullptr;

dir_entry *find(const utils::string &name, const dir_entry *parent) {
    for (auto entry : parent->dir_entries) {
        if (entry->name == name) {
            return entry;
        }
    }
    return nullptr;
}

dir_entry *find(const vnode_t &node, utils::list<dir_entry *> *list) {
    if (list == nullptr) {
        if (root->node == node.get()) {
            return root;
        }
        if (root->dir_entries.size() == 0) {
            return nullptr;
        }
        list = &root->dir_entries;
    }
    for (auto entry : *list) {
        debug("looking at ", (const char *)entry->name);
        if (entry->node == nullptr) {
            warning("cache entry is null");
            continue; // FIXME
        }
        if (entry->node == node.get()) {
            debug("\"", (const char *)entry->name, "\" found in cache");
            return entry;
        }
        if (entry->node->node_type == vnode::type::dir) {
            debug("going to dir ", (const char *)entry->name);
            auto result = find(node, &entry->dir_entries);
            if (result) {
                return result;
            }
        }
        else {
            debug((const char *)entry->name, " is a file");
        }
    }
    warning("no cache for node");
    return nullptr;
}

void add(const utils::string &name, vnode_t &vnode, dir_entry *parent) {
    parent->dir_entries.push_back(new dir_entry(name, vnode, parent));
}

} // namespace cache

vnode_t lookup(const path_t &path) {
    auto fs = mount_points.front()->fs;
    if (cache::root == nullptr) {
        auto root_node = fs->lookup("");
        vnodes.push_front(root_node);
        cache::root = new cache::dir_entry("", root_node.get());
    }
    if (path == "") {
        return cache::root->node;
    }
    auto path_it = path.begin();
    vnode_t node;
    auto parent_entry = cache::root;
    while (path_it) {
        auto name = *path_it;
        auto child_entry = cache::find(name, parent_entry);
        if (child_entry == nullptr) {
            node = fs->lookup(utils::path(name), node);
            if (!node) {
                return {};
            }
            vnodes.push_back(node);
            cache::add(name, node, parent_entry);
        }
        else {
            node = child_entry->node;
            if (!node) {
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

vnode_t create(const path_t &path, vnode::type type) {
    auto dir_node = lookup(utils::path(path.dirname()));
    if (not dir_node) {
        return {};
    }
    utils::path filename(path.basename());
    auto new_node = dir_node->fs->create(filename, dir_node, type);
    if (not new_node) {
        return {};
    }
    if (type == vnode::type::file) {
        warning("creating file ", path.get());
    }
    else {
        warning("creating dir ", path.get());
    }
    new_node->node_type = type;
    vnodes.push_back(new_node);
    auto cache_parent = cache::find(dir_node);
    if (not cache_parent) {
        if (cache_parent->name != path.dirname()) {
            warning("something is broken");
        }
        warning("parent node for ", (const char *)path, " isn\'t cached");
        return new_node;
    }
    cache::add(filename.get(), new_node, cache_parent);
    return new_node;
}

} // namespace vfs

