#pragma once

#include <list.hpp>
#include <string.hpp>
#include <unique_ptr.hpp>

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

        ~dir_entry() {
            for (auto &entry : dir_entries) {
                delete entry;
            }
        }

    };

private:

    utils::unique_ptr<dir_entry> root_;

public:

    dir_entry *add(const utils::string &name, vnode_t &vnode, dir_entry *parent = nullptr);
    dir_entry *find(const utils::string &name, const dir_entry *parent = nullptr);
    dir_entry *find(const vnode_t &node, utils::list<dir_entry *> *list = nullptr);
    bool empty() const;

};

} // namespace vfs

