#pragma once

#include "vfs.hpp"

namespace vfs {

struct file_system;

struct vnode final {

    enum class type {
        file, dir
    };

    unsigned id = 0;
    type node_type;
    size_t size = 0;
    size_t blocks = 0;
    void *data = nullptr;
    file_system *fs = nullptr;

    vnode(unsigned i, size_t s, size_t b, void *d, file_system *f, type t = type::file)
            : id(i), node_type(t), size(s), blocks(b), data(d), fs(f) {
    }

};

vnode_t lookup(const path_t &path);
vnode_t create(const path_t &path, vnode::type type = vnode::type::file);

} // namespace vfs

