#include <kernel/console/logger.hpp>
#include "cache.hpp"

namespace vfs {

namespace cache {

utils::unique_ptr<dir_entry> root;

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
    debug("adding ", (const char *)name);
    if (parent == nullptr) {
        cache::root = new cache::dir_entry("/", vnode);
    }
    else {
        parent->dir_entries.push_back(new dir_entry(name, vnode, parent));
    }
}

bool empty() {
    return root == nullptr;
}

} // namespace cache

} // namespace vfs

