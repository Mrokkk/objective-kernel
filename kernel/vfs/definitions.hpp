#pragma once

#include <list.h>
#include <path.h>
#include <shared_ptr.h>

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

