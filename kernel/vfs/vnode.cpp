#include <list.h>
#include <unique_ptr.h>

#include <kernel/console/logger.hpp>

#include "vfs.hpp"
#include "vnode.hpp"
#include "cache.hpp"
#include "file_system.hpp"

namespace vfs {

extern utils::unique_ptr<vfs> vfs_;

const char *to_string(vnode::type type) {
    switch (type) {
        case vnode::type::file:
            return "file";
        case vnode::type::dir:
            return "dir";
        default:
            return "unknown type";
    }
}

vnode_t lookup(const path_t &path) {
    if (not vfs_) return {};
    return vfs_->lookup(path);
}

vnode_t create(const path_t &path, vnode::type type) {
    if (not vfs_) return {};
    return vfs_->create(path, type);
}

} // namespace vfs

