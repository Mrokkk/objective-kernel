#pragma once

#include <list.h>
#include <string.h>

#include "vfs.hpp"
#include "vnode.hpp"
#include "file_system.hpp"

namespace ramfs {

class ramfs final : public vfs::file_system {

    unsigned node_nr = 1;

    struct dir_entry {

        unsigned id = 0;
        utils::string file_name;
        vfs::vnode::type file_type;
        char *content = nullptr;
        size_t size;
        utils::list<dir_entry *> dir_entries;
        vfs::file_system *fs = nullptr;

        dir_entry(unsigned id, const utils::string &name, vfs::vnode::type t,
            vfs::file_system *f, char *c = nullptr, size_t s = 0)
                : id(id), file_name(name), file_type(t), content(c), size(s), fs(f) {
        }

        ~dir_entry() {
            if (content) {
                delete [] content;
            }
        }

    };

    dir_entry root_;

    dir_entry *lookup_in_dir(utils::list<dir_entry *> &dir, const utils::string &name);
    dir_entry *dir_entry_lookup(const vfs::path_t &path);

public:

    ramfs() : root_(1, "", vfs::vnode::type::dir, this) {
    }

    utils::string name() override;
    vfs::vnode_t lookup(const vfs::path_t &path, vfs::vnode_t parent) override;
    vfs::vnode_t create(const vfs::path_t &path, vfs::vnode_t parent, vfs::vnode::type type) override;
    int read(vfs::vnode &vnode, char *buffer, size_t size = 0) override;
    int write(vfs::vnode &vnode, const char *buffer, size_t size) override;
    void sync(vfs::vnode &vnode) override;

};

} // namespace ramfs

