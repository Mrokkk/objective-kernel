#include <list.hpp>
#include <unique_ptr.hpp>

#include <kernel/console/logger.hpp>

#include "vfs.hpp"
#include "vnode.hpp"
#include "cache.hpp"
#include "file_system.hpp"

namespace vfs {

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

} // namespace vfs

