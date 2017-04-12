#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/file.hpp>

#include "yatf/include/yatf.h"

namespace test_cases {

TEST(vfs, can_create_root) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    auto node = vfs::lookup("");
    REQUIRE(node);
    REQUIRE_EQ(node->id, 1u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    REQUIRE_EQ(node->size, 0u);
}

TEST(vfs, can_create_files) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    auto node = vfs::create("/some_file");
    REQUIRE(node);
    REQUIRE_EQ(node->id, 2u);
    REQUIRE(node->node_type == vfs::vnode::type::file);
    // TODO: check size
    {
        auto node2 = vfs::lookup("/some_file");
        REQUIRE_EQ(node2->id, 2u);
        REQUIRE(node2->node_type == vfs::vnode::type::file);
    }
    node = vfs::create("/some_file2");
    REQUIRE(node);
    REQUIRE_EQ(node->id, 3u);
    REQUIRE(node->node_type == vfs::vnode::type::file);
    {
        auto node2 = vfs::lookup("/some_file2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE(node2->node_type == vfs::vnode::type::file);
    }
}

TEST(vfs, can_create_dirs) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    auto node = vfs::create("/some_dir", vfs::vnode::type::dir);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 2u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    {
        auto node2 = vfs::lookup("/some_dir");
        REQUIRE_EQ(node2->id, 2u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
    node = vfs::create("/some_dir2", vfs::vnode::type::dir);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 3u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    {
        auto node2 = vfs::lookup("/some_dir2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
}

TEST(vfs, can_cache_nodes) {
    // TODO
}

} // namespace test_cases

