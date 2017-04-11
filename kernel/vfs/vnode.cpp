#include <list.h>

#include "vfs.hpp"
#include "vnode.hpp"
#include "file_system.hpp"

namespace vfs {

extern utils::list<mount_point *> mount_points;
extern utils::list<vnode_t> vnodes;

vnode_t lookup(const path_t &path) {
    auto fs = mount_points.front()->fs;
    if (path == "") {
        return fs->lookup("");
    }
    auto path_it = path.begin();
    vnode_t node;
    while (path_it) {
        node = fs->lookup(utils::path(*path_it), node);
        if (!node) {
            return {};
        }
        if (node->fs != fs) {
            if (node->fs == nullptr) {
                return {};
            }
            fs = node->fs;
        }
        ++path_it;
    }
    return node;
}

vnode_t create(const path_t &path, vnode::type type) {
    auto dir_node = lookup(utils::path(path.dirname()));
    if (!dir_node) {
        return {};
    }
    utils::path filename(path.basename());
    switch (type) {
        case vnode::type::file:
            return dir_node->fs->create_file(filename, dir_node);
        case vnode::type::dir:
            return dir_node->fs->create_dir(filename, dir_node);
        default:
            return {};
    }
}

} // namespace vfs

