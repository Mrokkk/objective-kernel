#pragma once

#include "vfs.hpp"

namespace vfs {

struct file_system;

struct vnode final {

    enum class type {
        file, dir
    };

    unsigned id = 0;
    size_t size = 0;
    size_t blocks = 0;
    uint32_t data = 0;
    file_system *fs = nullptr;

    vnode(unsigned i, size_t s, size_t b, uint32_t d, file_system *f)
            : id(i), size(s), blocks(b), data(d), fs(f) {
    }

};

utils::shared_ptr<vnode> lookup(const path_t &path);

} // namespace vfs

