#pragma once

#include "vfs.hpp"
#include "vnode.hpp"

namespace vfs {

struct file_system {
    virtual utils::string name() = 0;
    virtual vnode_t lookup(const path_t &path, vnode_t parent = nullptr) = 0;
    virtual vnode_t create(const path_t &path, vnode_t parent, vnode::type type = vnode::type::file) = 0;
    virtual int read(vnode &vnode, char *buffer, size_t size = 0) = 0;
    virtual int write(vnode &vnode, const char *buffer, size_t size) = 0;
    virtual void sync(vnode &vnode) = 0;
};

} // namespace vfs
