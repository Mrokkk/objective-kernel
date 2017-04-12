#include <list.h>

#include "vfs.hpp"
#include "vnode.hpp"
#include "dir_entry.hpp"
#include "file_system.hpp"

#include <kernel/console/console.hpp>

namespace vfs {

utils::list<vnode_t> vnodes;
extern utils::list<mount_point *> mount_points;

namespace cache {

dir_entry *root = nullptr;

dir_entry *find(const utils::string &name, dir_entry *parent) {
    for (auto entry : parent->dir_entries) {
        if (entry->name == name) {
            return entry;
        }
    }
    return nullptr;
}

void add(const utils::string &name, vnode_t vnode, dir_entry *parent) {
    parent->dir_entries.push_back(new dir_entry(name, vnode));
}

} // namespace cache

vnode_t lookup(const path_t &path) {
    auto fs = mount_points.front()->fs;
    if (path == "") {
        return fs->lookup("");
    }
    if (cache::root == nullptr) {
        auto root_node = fs->lookup("");
        vnodes.push_front(root_node);
        cache::root = new cache::dir_entry("", root_node.get());
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
                console::print("Node is null\n");
            }
        }
        if (node->fs != fs) {
            if (node->fs == nullptr) {
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
    if (!dir_node) {
        return {};
    }
    utils::path filename(path.basename());
    return dir_node->fs->create(filename, dir_node, type);
}

} // namespace vfs

