#include "file.hpp"
#include "ramfs.hpp"
#include <kernel/console/console.hpp>

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
    auto list = &root_;
    auto path_it = path.cbegin();
    if (root_.size() == 0) return nullptr;
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

vfs::vnode_t ramfs::lookup(const vfs::path_t &path) {
    auto entry = dir_entry_lookup(path);
    if (entry) {
        //console::print("  found!!!!\n");
        return new vfs::vnode(entry->id, entry->size, 1u, reinterpret_cast<uint32_t>(entry), this);
    }
    return {};
}

vfs::vnode_t ramfs::create_file(const vfs::path_t &path) {
    auto dirname = path.dirname();
    auto filename = path.basename();
    if (dirname == "") {
        auto content = new char[32];
        auto entry = new dir_entry(node_nr++, filename, vfs::vnode::type::file, content);
        root_.push_back(entry);
        return new vfs::vnode(entry->id, 0u, 1u, reinterpret_cast<uint32_t>(entry), this);
    }
    else {
        auto dir_node = dir_entry_lookup((const char *)dirname);
        if (dir_node == nullptr) {
            return {};
        }
        auto content = new char[32];
        auto entry = new dir_entry(node_nr++, filename, vfs::vnode::type::file, content, 32);
        dir_node->dir_entries.push_back(entry);
        return new vfs::vnode(entry->id, 0u, 1u, reinterpret_cast<uint32_t>(entry), this);
    }
    return {};
}

vfs::vnode_t ramfs::create_dir(const vfs::path_t &path) {
    auto dirname = path.dirname();
    auto filename = path.basename();
    if (dirname == "") {
        auto entry = new dir_entry(node_nr++, filename, vfs::vnode::type::dir, nullptr);
        root_.push_back(entry);
        return new vfs::vnode(entry->id, 0u, 1u, reinterpret_cast<uint32_t>(entry), this, vfs::vnode::type::dir);
    }
    else {
        auto dir_node = dir_entry_lookup((const char *)dirname);
        if (dir_node == nullptr) {
            return {};
        }
        auto entry = new dir_entry(node_nr++, filename, vfs::vnode::type::dir, nullptr);
        dir_node->dir_entries.push_back(entry);
        return new vfs::vnode(entry->id, 0u, 1u, reinterpret_cast<uint32_t>(entry), this, vfs::vnode::type::dir);
    }
    return {};
}

void ramfs::sync(vfs::vnode &) {
}

int ramfs::read(vfs::vnode &vnode, char *buffer, size_t size) {
    if (size == 0) size = vnode.size;
    auto node = reinterpret_cast<dir_entry *>(vnode.data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    utils::memcopy(node->content, buffer, size);
    return size;
}

int ramfs::write(vfs::vnode &vnode, const char *buffer, size_t size) {
    auto node = reinterpret_cast<dir_entry *>(vnode.data);
    if (node->file_type != vfs::vnode::type::file) return 0;
    utils::memcopy(buffer, node->content, size);
    vnode.size = size;
    node->size = size;
    return size;
}

} // namespace ramfs

