#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/dir_entry.hpp>

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
    auto node = vfs::create("/some_file", vfs::vnode::type::file);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 2u);
    REQUIRE(node->node_type == vfs::vnode::type::file);
    // TODO: check size
    {
        auto node2 = vfs::lookup("/some_file");
        REQUIRE_EQ(node2->id, 2u);
        REQUIRE(node2->node_type == vfs::vnode::type::file);
    }
    node = vfs::create("/some_file2", vfs::vnode::type::file);
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
    node = create("/some_dir2/file", vfs::vnode::type::file);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 4u);
    REQUIRE(node->node_type == vfs::vnode::type::file);
    {
        auto node2 = vfs::lookup("/some_dir2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
    {
        auto node2 = vfs::lookup("/some_dir2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
}

TEST(vfs, can_cache_nodes) {
    ramfs::ramfs ramfs;
    vfs::initialize(ramfs);
    {
        auto orig_node = vfs::create("/some_file", vfs::vnode::type::file);
        auto cache_entry = vfs::cache::find(orig_node);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_file");
    }
    {
        auto orig_node = vfs::create("/some_dir", vfs::vnode::type::dir);
        auto cache_entry = vfs::cache::find(orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_dir");
        REQUIRE(cache_entry->node->node_type == vfs::vnode::type::dir);
    }
    {
        auto orig_node = vfs::create("/some_dir/some_other_file", vfs::vnode::type::file);
        auto cache_entry = vfs::cache::find(orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_other_file");
    }
    {
        auto orig_node = vfs::create("/some_dir/other_file", vfs::vnode::type::file);
        auto node = vfs::lookup("/some_dir");
        auto cache_entry = vfs::cache::find(orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(node->node_type == vfs::vnode::type::dir);
    }
    {
        auto node = vfs::lookup("/some_dir");
        REQUIRE(node);
        REQUIRE(node->node_type == vfs::vnode::type::dir);
    }
}

} // namespace test_cases

