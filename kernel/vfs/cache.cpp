#include <kernel/console/logger.hpp>
#include "cache.hpp"

namespace vfs {

cache::cache() {
    logger_.initialize("vfs.cache");
}

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
        logger_ << logger::log_level::debug << "looking at " << entry->name << "\n";
        if (entry->node == nullptr) {
            logger_ << logger::log_level::warning << "cache entry is null\n";
            continue; // FIXME
        }
        if (entry->node == node.get()) {
            logger_ << logger::log_level::debug << "\"" << entry->name << "\" found in cache" << "\n";
            return entry;
        }
        if (entry->node->node_type == vnode::type::dir) {
            logger_ << logger::log_level::debug << "going to dir " << entry->name << "\n";
            auto result = find(node, &entry->dir_entries);
            if (result) {
                return result;
            }
        }
        else {
            logger_ << logger::log_level::debug << entry->name << " is a file" << "\n";
        }
    }
    logger_ << logger::log_level::warning << "no cache for node\n";
    return nullptr;
}

cache::dir_entry *cache::add(const utils::string &name, vnode_t &vnode, dir_entry *parent) {
    logger_ << logger::log_level::debug << "adding " << name << "\n";
    if (parent == nullptr) {
        root_ = new cache::dir_entry("/", vnode);
        return root_;
    }
    else {
        auto new_cache_entry = new dir_entry(name, vnode, parent);
        parent->dir_entries.push_back(new_cache_entry);
        return new_cache_entry;
    }
}

bool cache::empty() const {
    return root_ == nullptr;
}

} // namespace vfs

