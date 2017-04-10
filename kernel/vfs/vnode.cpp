#include "vfs.hpp"
#include "vnode.hpp"

#include <list.h>

namespace vfs {

extern utils::list<mount_point *> mount_points;

vnode_t lookup(const path_t &path) {
    // TODO
    return mount_points.front()->fs->lookup(path);
}

vnode_t create(const path_t &path, vnode::type) {
    // TODO
    return mount_points.front()->fs->create(path);
}

} // namespace vfs

