#include <kernel/vfs/ramfs.hpp>
#include "yatf/include/yatf.h"

namespace test_cases {

TEST(ramfs, can_create_nodes) {
    ramfs::ramfs ramfs;
    REQUIRE_EQ((const char *)ramfs.name(), "ramfs");
    REQUIRE_FALSE(ramfs.lookup("file", nullptr));
    REQUIRE_FALSE(ramfs.lookup("/file", nullptr));
    REQUIRE_FALSE(ramfs.lookup("dir/file", nullptr));
    REQUIRE_FALSE(ramfs.lookup("/dir/file", nullptr));
    auto root = ramfs.lookup("/", nullptr);
    REQUIRE(root);
    REQUIRE(root->node_type == vfs::vnode::type::dir);
    REQUIRE_EQ(root->id, 1u);
    auto file_node = ramfs.create("file", root, vfs::vnode::type::file);
    REQUIRE(file_node);
    REQUIRE(file_node->node_type == vfs::vnode::type::file);
    REQUIRE_EQ(file_node->id, 2u);
    REQUIRE(ramfs.lookup("file", root));
    auto dir_node = ramfs.create("dir", root, vfs::vnode::type::dir);
    REQUIRE(dir_node);
    REQUIRE(dir_node->node_type == vfs::vnode::type::dir);
    REQUIRE_EQ(dir_node->id, 3u);
    REQUIRE_EQ(dir_node->size, 0u);
}

} // namespace test_cases

