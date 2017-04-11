#pragma once

#include <list.h>
#include <string.h>

namespace vfs {

struct vnode;

namespace cache {

struct dir_entry {

    utils::string name;
    vnode *node = nullptr;
    dir_entry *parent = nullptr;
    utils::list<dir_entry *> dir_entries;

    dir_entry(const utils::string &name, vnode *node, dir_entry *parent = nullptr)
            : name(name), node(node), parent(parent) {
    }

};


dir_entry *find(const utils::string &name, dir_entry *parent);

} // namespace cache

} // namespace vfs

