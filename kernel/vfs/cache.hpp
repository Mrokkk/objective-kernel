#pragma once

#include <list.h>
#include <string.h>
#include <unique_ptr.h>

#include "vnode.hpp"

namespace vfs {

struct cache {

    struct dir_entry {

        utils::string name;
        vnode_t node;
        dir_entry *parent;
        utils::list<dir_entry *> dir_entries;

        dir_entry(const utils::string &name, vnode_t node, dir_entry *parent = nullptr)
                : name(name), node(node), parent(parent) {
        }

    };

private:

    utils::unique_ptr<dir_entry> root_;

public:

    void add(const utils::string &name, vnode_t &vnode, dir_entry *parent = nullptr);
    dir_entry *find(const utils::string &name, const dir_entry *parent);
    dir_entry *find(const vnode_t &node, utils::list<dir_entry *> *list = nullptr);
    bool empty();
    dir_entry *root();

};

} // namespace vfs

