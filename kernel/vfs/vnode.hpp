#pragma once

#include "definitions.hpp"
#include "mount_point.hpp"
#include <kernel/error.hpp>

namespace vfs {

struct vnode final {

    enum class type {
        file = 1, dir
    };

    unsigned id = 0;
    type node_type;
    size_t size = 0;
    size_t blocks = 0;
    void *data = nullptr;
    mount_point_t fs;


    vnode(unsigned id, size_t size, size_t blocks, void *data, type type)
            : id(id), node_type(type), size(size), blocks(blocks), data(data) {
    }

};

const char *to_string(vnode::type type);
vnode_t lookup(const path_t &path);
vnode_t create(const path_t &path, vnode::type type);

} // namespace vfs

