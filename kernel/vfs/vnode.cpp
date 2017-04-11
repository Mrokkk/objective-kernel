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
    utils::path temp_path;
    vnode_t node;
    while (path_it) {
        temp_path.append(*path_it);
        node = fs->lookup(temp_path);
        if (!node) {
            return {};
        }
        if (node->fs != fs) {
            if (node->fs == nullptr) {
                return {};
            }
            fs = node->fs;
            temp_path = utils::path();
        }
        ++path_it;
    }
    return node;
}

vnode_t create(const path_t &path, vnode::type type) {
    // FIXME
    auto dir_node = lookup(utils::path(path.dirname()));
    utils::path filename(path.basename());
    switch (type) {
        case vnode::type::file:
            return dir_node->fs->create_file(path);
        case vnode::type::dir:
            return dir_node->fs->create_dir(path);
        default:
            return {};
    }
}

} // namespace vfs

