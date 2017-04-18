#include <kernel/console/logger.hpp>
#include "cache.hpp"

namespace vfs {

cache::dir_entry *cache::find(const utils::string &name, const dir_entry *parent) {
    if (not parent) {
        if (not root_) {
            return nullptr;
        }
        if (name == root_->name) {
            return root_;
        }
        else {
            return nullptr;
        }
    }
    for (auto entry : parent->dir_entries) {
        if (entry->name == name) {
            return entry;
        }
    }
    return nullptr;
}

cache::dir_entry *cache::find(const vnode_t &node, utils::list<dir_entry *> *list) {
    if (list == nullptr) {
        if (root_->node == node.get()) {
            return root_;
        }
        if (root_->dir_entries.size() == 0) {
            return nullptr;
        }
        list = &root_->dir_entries;
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

void cache::add(const utils::string &name, vnode_t &vnode, dir_entry *parent) {
    debug("adding ", (const char *)name);
    if (parent == nullptr) {
        root_ = new cache::dir_entry("/", vnode);
    }
    else {
        parent->dir_entries.push_back(new dir_entry(name, vnode, parent));
    }
}

bool cache::empty() const {
    return root_ == nullptr;
}

} // namespace vfs

