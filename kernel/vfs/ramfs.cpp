#include "file.hpp"
#include "ramfs.hpp"

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
            return new vfs::vnode(entry->id, entry->size, 1u, static_cast<void *>(entry), entry->fs);
        }
    }
    else {
        auto dir_node = static_cast<dir_entry *>(parent->data);
        auto entry = lookup_in_dir(dir_node->dir_entries, path.get());
        if (entry) {
            return new vfs::vnode(entry->id, entry->size, 1u, static_cast<void *>(entry), entry->fs);
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
    return new vfs::vnode(new_entry->id, 0u, 1u, static_cast<void *>(new_entry), this, type);
}

void ramfs::sync(vfs::vnode &vnode) {
    if (static_cast<dir_entry *>(vnode.data)->fs != this) {
        // TODO
    }
    static_cast<dir_entry *>(vnode.data)->fs = vnode.fs;
}

int ramfs::read(vfs::file *, vfs::vnode_t &vnode, char *buffer, size_t size) {
    if (size == 0) size = vnode->size;
    auto node = reinterpret_cast<dir_entry *>(vnode->data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    utils::memcopy(node->content, buffer, size);
    return size;
}

int ramfs::write(vfs::file *, vfs::vnode_t &vnode, const char *buffer, size_t size) {
    auto node = reinterpret_cast<dir_entry *>(vnode->data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    utils::memcopy(buffer, node->content, size);
    vnode->size = size;
    node->size = size;
    return size;
}

} // namespace ramfs

