#pragma once

#include <list.h>
#include <string.h>
#include "vfs.hpp"

namespace dummyfs {

class dummyfs {

    struct dir_entry {

        utils::string file_name;
        vfs::vnode::type file_type;
        char *content = nullptr;
        size_t size;
        utils::list<dir_entry *> dir_entries;

        dir_entry(const utils::string &name, vfs::vnode::type t, char *c = nullptr, size_t s = 0)
                : file_name(name), file_type(t), content(c), size(s) {
        }

        ~dir_entry() {
            if (content) {
                delete [] content;
            }
        }

    };

    utils::list<dir_entry *> root_;

    dir_entry *create_dir(const utils::string &name) {
        return new dir_entry(name, vfs::vnode::type::dir);
    }

    dir_entry *lookup_in_dir(utils::list<dir_entry *> &dir, const utils::string &name) {
        for (const auto &entry : dir) {
            if (entry->file_name == name) {
                return entry;
            }
        }
        return nullptr;
    }

    dir_entry *dir_entry_lookup(const utils::path &path) {
        auto list = &root_;
        auto path_it = path.cbegin();
        if (root_.size() == 0) return nullptr;
        while (true) {
            auto node = lookup_in_dir(*list, *path_it);
            if (node == nullptr) return nullptr;
            if (++path_it) {
                break;
            }
            else {
                return node;
            }
        }
        return nullptr;
    }

public:

    dummyfs() {
    }

    vfs::vnode lookup(const utils::path &path) {
        auto entry = dir_entry_lookup(path);
        if (entry) {
            return vfs::vnode(entry->size, 1u, reinterpret_cast<uint32_t>(entry));
        }
        return {};
    }

    vfs::vnode create(const utils::path &path, vfs::vnode::type = vfs::vnode::type::file) {
        auto dirname = path.dirname();
        auto filename = path.basename();
        if (dirname == "") {
            auto content = new char[32];
            auto entry = new dir_entry(filename, vfs::vnode::type::file, content);
            root_.push_back(entry);
            return vfs::vnode(0u, 1u, reinterpret_cast<uint32_t>(entry));
        }
        else {
            auto dir_node = dir_entry_lookup((const char *)dirname);
            if (dir_node == nullptr) {
                return {};
            }
            auto content = new char[32];
            auto entry = new dir_entry(filename, vfs::vnode::type::file, content, 32);
            dir_node->dir_entries.push_back(entry);
            return vfs::vnode(0u, 1u, reinterpret_cast<uint32_t>(entry));
        }
        return {};
    }

    int read(vfs::vnode &vnode, char *buffer, size_t size = 0) {
        if (size == 0) size = vnode.size;
        auto node = reinterpret_cast<dir_entry *>(vnode.data);
        if (node->file_type != vfs::vnode::type::file) return 0;
        utils::memcopy(node->content, buffer, size);
        return size;
    }

    int write(vfs::vnode &vnode, const char *buffer, size_t size) {
        auto node = reinterpret_cast<dir_entry *>(vnode.data);
        if (node->file_type != vfs::vnode::type::file) return 0;
        utils::memcopy(buffer, node->content, size);
        vnode.size = size;
        node->size = size;
        return size;
    }

};

} // namespace dummyfs

