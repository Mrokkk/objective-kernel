#pragma once

#include <list.h>
#include <string.h>

#include "vnode.hpp"

namespace vfs {

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

dir_entry *find(const utils::string &name, const dir_entry *parent);
dir_entry *find(const vnode_t &node, utils::list<dir_entry *> *list = nullptr);

} // namespace cache

} // namespace vfs

