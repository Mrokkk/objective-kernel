#include "file.hpp"
#include "ramfs.hpp"

#include <kernel/console/logger.hpp>

namespace ramfs {

ramfs::dir_entry *ramfs::lookup_in_dir(utils::list<dir_entry *> &dir, const utils::string &name) {
    for (const auto &entry : dir) {
        if (entry->file_name == name) {
            return entry;
        }
    }
    return nullptr;
}

ramfs::dir_entry *ramfs::dir_entry_lookup(const vfs::path_t &path) {
    if (path == "") {
        return &root_;
    }
    auto list = &root_.dir_entries;
    auto path_it = path.cbegin();
    if (root_.dir_entries.size() == 0) return nullptr;
    while (true) {
        auto node = lookup_in_dir(*list, *path_it);
        if (node == nullptr) return nullptr;
        if (!++path_it) {
            return node;
        }
        else {
            if (node->file_type == vfs::vnode::type::dir) {
                list = &node->dir_entries;
            }
            else {
                return nullptr;
            }
        }
    }
    return nullptr;
}

utils::string ramfs::name() {
    return "ramfs";
}

vfs::vnode_t ramfs::lookup(const vfs::path_t &path, vfs::vnode_t parent) {
    if (!parent) {
        auto entry = dir_entry_lookup(path);
        if (entry) {
            return utils::make_shared<vfs::vnode>(entry->id, entry->size, 1u, static_cast<void *>(entry), entry->fs, entry->file_type);
        }
    }
    else {
        auto dir_node = static_cast<dir_entry *>(parent->data);
        auto entry = lookup_in_dir(dir_node->dir_entries, path.get());
        if (entry) {
            return utils::make_shared<vfs::vnode>(entry->id, entry->size, 1u, static_cast<void *>(entry), entry->fs, entry->file_type);
        }
    }
    return {};
}

vfs::vnode_t ramfs::create(const vfs::path_t &path, vfs::vnode_t parent, vfs::vnode::type type) {
    auto filename = path.get();
    char *content = nullptr;
    if (!parent) {
        return {};
    }
    auto entry = static_cast<dir_entry *>(parent->data);
    if (type == vfs::vnode::type::file) {
        content = new char[32];
    }
    auto new_entry = new dir_entry(node_nr++, filename, type, this, content);
    entry->dir_entries.push_back(new_entry);
    return utils::make_shared<vfs::vnode>(new_entry->id, 0u, 1u, static_cast<void *>(new_entry), this, type);
}

void ramfs::sync(vfs::vnode &vnode) {
    if (static_cast<dir_entry *>(vnode.data)->fs != this) {
        warning("i'm not the owner of this node!");
    }
    static_cast<dir_entry *>(vnode.data)->fs = vnode.fs;
}

int ramfs::read(vfs::file *file, vfs::vnode_t &vnode, char *buffer, size_t size) {
    if (size == 0u) {
        size = vnode->size;
    }
    auto node = reinterpret_cast<dir_entry *>(vnode->data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    auto pos = file->position();
    utils::memcopy(node->content + pos, buffer, size);
    pos += size;
    file->position(pos);
    return size;
}

int ramfs::write(vfs::file *file, vfs::vnode_t &vnode, const char *buffer, size_t size) {
    auto node = reinterpret_cast<dir_entry *>(vnode->data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    auto pos = file->position();
    utils::memcopy(buffer, node->content + pos, size);
    vnode->size = size;
    node->size = size;
    pos += size;
    file->position(pos);
    return size;
}

} // namespace ramfs

