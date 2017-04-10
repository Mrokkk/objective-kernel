#include "vfs.hpp"
#include "vnode.hpp"
#include <kernel/console/console.hpp>

#include <list.h>

namespace vfs {

extern utils::list<mount_point *> mount_points;
extern utils::list<vnode_t> vnodes;

vnode_t lookup(const path_t &path) {
    auto fs = mount_points.front()->fs;
    auto path_it = path.begin();
    utils::path temp_path;
    vnode_t node;
    while (path_it) {
        temp_path.append(*path_it);
        node = fs->lookup(temp_path);
        if (!node) {
            return {};
        }
        if (node->fs != fs) {
            fs = node->fs;
            temp_path = utils::path();
        }
        ++path_it;
    }
    return node;
}

vnode_t create(const path_t &path, vnode::type type) {
    switch (type) {
        case vnode::type::file:
            return mount_points.front()->fs->create_file(path);
        case vnode::type::dir:
            return mount_points.front()->fs->create_dir(path);
        default:
            return {};
    }
}

} // namespace vfs

