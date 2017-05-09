#pragma once

#include <list.hpp>
#include <path.hpp>
#include <shared_ptr.hpp>

namespace vfs {

struct file;
struct vnode;
struct file_system;
class mount_point;

using path_t = utils::path;
using vnode_t = utils::shared_ptr<vnode>;
using file_t = utils::shared_ptr<file>;
using mount_point_t = utils::shared_ptr<mount_point>;

} // namespace vfs

