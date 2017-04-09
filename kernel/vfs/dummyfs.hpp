#pragma once

#include <list.h>
#include <string.h>
#include "vfs.hpp"
#include <kernel/console.h>

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

public:

    dummyfs() {
        auto dummy_content = new char[32];
        utils::copy("hello world\n", dummy_content);
        root_.push_back(new dir_entry("file1", dir_entry::type::file, dummy_content, utils::length(dummy_content) + 1));
        root_.push_back(new dir_entry("file2", dir_entry::type::file, dummy_content, utils::length(dummy_content) + 1));
        root_.push_back(new dir_entry("dummy", dir_entry::type::file, dummy_content, utils::length(dummy_content) + 1));
        auto dir = create_dir("dir");
        auto content = new char[32];
        utils::copy("This is kernel\n", content);
        dir->dir_entries.push_back(new dir_entry("dummy", dir_entry::type::file, content, utils::length(content)));
        root_.push_back(dir);
    }

    vfs::vnode lookup(const utils::path &path) {
        console::print("Looking for ");
        console::print((const char *)(path));
        console::print("\n");
        auto list = &root_;
        auto path_it = path.cbegin();
        while (true) {
            for (const auto &entry : *list) {
                console::print("Looking at ");
                console::print((const char *)entry->file_name);
                console::print("\n");
                if (entry->file_name == *path_it) {
                    if (++path_it) {
                        list = &entry->dir_entries;
                        break;
                    }
                    else {
                        return vfs::vnode(entry->size, 1u, reinterpret_cast<uint32_t>(entry->content));
                    }
                }
            }
        }
        return {};
    }

    vfs::vnode create(const utils::path &path) {
        auto basename = path.dirname();
        if (basename == "") {
            root_.push_back(new dir_entry(path.get(), dir_entry::type::file));
        }
        else {
            // TODO
        }
        return {};
    }

    int read(vfs::vnode &vnode, char *buffer, size_t size = 0) {
        if (size == 0) size = vnode.size;
        utils::memcopy(reinterpret_cast<char *>(vnode.data), buffer, size);
        return size;
    }

    int write(vfs::vnode &vnode, const char *buffer, size_t size) {
        utils::memcopy(buffer, reinterpret_cast<char *>(vnode.data), size);
        return size;
    }

};

} // namespace dummyfs

