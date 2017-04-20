#include <kernel/vfs/ramfs.hpp>
#include <yatf.h>

namespace {

void lookup_for(ramfs::ramfs &ramfs, const std::initializer_list<utils::string> &names) {
    for (const auto &name : names) {
        REQUIRE(ramfs.lookup(vfs::path_t(name), ramfs.lookup("/", nullptr)));
    }
}

} // namespace

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
    auto file_under_dir_node = ramfs.create("file", dir_node, vfs::vnode::type::file);
    REQUIRE(file_under_dir_node);
    REQUIRE(file_under_dir_node->node_type == vfs::vnode::type::file);
    REQUIRE_EQ(file_under_dir_node->id, 4u);
    REQUIRE_EQ(file_under_dir_node->size, 0u);
    auto file2_under_dir_node = ramfs.create("file2", dir_node, vfs::vnode::type::file);
    REQUIRE(file2_under_dir_node);
    REQUIRE(file2_under_dir_node->node_type == vfs::vnode::type::file);
    REQUIRE_EQ(file2_under_dir_node->id, 5u);
    REQUIRE_EQ(file2_under_dir_node->size, 0u);
    lookup_for(ramfs, {"file", "dir"});
}

} // namespace test_cases

