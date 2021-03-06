#pragma once

#include "definitions.hpp"
#include "mount_point.hpp"

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
    class mount_point *mount_point = nullptr;

    vnode(unsigned id, size_t size, size_t blocks, void *data, type type)
            : id(id), node_type(type), size(size), blocks(blocks), data(data) {
    }

};

const char *to_string(vnode::type type);

} // namespace vfs

