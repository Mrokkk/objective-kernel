#include <cstdint>
#include <kernel/definitions.hpp>
#include <kernel/cpu/common.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/cache.hpp>

#include <yatf.h>

TEST(vfs, have_to_be_initialized) {
    REQUIRE_FALSE(vfs::create("/file", vfs::vnode::type::file));
    REQUIRE_FALSE(vfs::lookup("/file"));
    REQUIRE_FALSE(vfs::lookup("/"));
}

TEST(vfs, can_create_root) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    auto node = vfs::lookup("/");
    REQUIRE(node);
    REQUIRE_EQ(node->id, 1u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    REQUIRE_EQ(node->size, 0u);
}

