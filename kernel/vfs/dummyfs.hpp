#pragma once

#include <list.h>
#include <string.h>
#include "vfs.hpp"

namespace dummyfs {

class dummyfs {

    struct dir_entry {

        enum class type {
            file, dir
        };

        utils::string file_name;
        type file_type;
        char *content = nullptr;
        size_t size;
        utils::list<dir_entry *> dir_entries;

        dir_entry(const utils::string &name, type t, char *c = nullptr, size_t s = 0) : file_name(name), file_type(t), content(c), size(s) {
        }

        ~dir_entry() {
            if (content) {
                delete [] content;
            }
        }

    };

    utils::list<dir_entry *> root_;

    dir_entry *create_dir(const utils::string &name) {
        return new dir_entry(name, dir_entry::type::dir);
    }

    dir_entry *dir_entry_lookup(const utils::path &path) {
        auto list = &root_;
        auto path_it = path.cbegin();
        if (root_.size() == 0) return nullptr;
        while (true) {
            for (const auto &entry : *list) {
                if (entry->file_name == *path_it) {
                    if (++path_it) {
                        list = &entry->dir_entries;
                        break;
                    }
                    else {
                        return entry;
                    }
                }
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

    vfs::vnode create(const utils::path &path) {
        auto dirname = path.dirname();
        auto filename = path.basename();
        console::print((const char *)dirname);
        if (dirname == "") {
            auto content = new char[32];
            auto entry = new dir_entry(filename, dir_entry::type::file, content);
            root_.push_back(entry);
            return vfs::vnode(0u, 1u, reinterpret_cast<uint32_t>(entry));
        }
        else {
            auto dir_node = dir_entry_lookup((const char *)dirname);
            if (dir_node == nullptr) {
                return {};
            }
            auto content = new char[32];
            auto entry = new dir_entry(filename, dir_entry::type::file, content, 32);
            dir_node->dir_entries.push_back(entry);
            return vfs::vnode(0u, 1u, reinterpret_cast<uint32_t>(entry));
        }
        return {};
    }

    int read(vfs::vnode &vnode, char *buffer, size_t size = 0) {
        if (size == 0) size = vnode.size;
        utils::memcopy(reinterpret_cast<dir_entry *>(vnode.data)->content, buffer, size);
        return size;
    }

    int write(vfs::vnode &vnode, const char *buffer, size_t size) {
        auto node = reinterpret_cast<dir_entry *>(vnode.data);
        utils::memcopy(buffer, node->content, size);
        vnode.size = size;
        node->size = size;
        return size;
    }

};

} // namespace dummyfs

