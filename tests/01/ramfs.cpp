#include <cstdint>
#include <kernel/definitions.hpp>
#include <kernel/cpu/common.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/cache.hpp>

#include <yatf.hpp>

struct ramfs_fixture {
    ramfs::ramfs fs;
};

TEST(ramfs, can_create, ramfs_fixture) {
    REQUIRE_EQ((const char *)fs.name(), "ramfs");
    auto root_node = fs.lookup("/", {});
    REQUIRE(root_node);
    REQUIRE_EQ(root_node->id, 1u);
}

TEST(ramfs, parent_node_has_to_be_passed_to_create, ramfs_fixture) {
    REQUIRE_FALSE(fs.create("file", {}, vfs::vnode::type::file));
    REQUIRE_FALSE(fs.create("dir", {}, vfs::vnode::type::dir));
}

TEST(ramfs, can_create_dir, ramfs_fixture) {
    auto root_node = fs.lookup("/", {});
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    REQUIRE(dir_node);
    REQUIRE(dir_node->node_type == vfs::vnode::type::dir);
}

TEST(ramfs, can_create_file, ramfs_fixture) {
    auto root_node = fs.lookup("/", {});
    auto dir_node = fs.create("file", root_node, vfs::vnode::type::file);
    REQUIRE(dir_node);
    REQUIRE(dir_node->node_type == vfs::vnode::type::file);
}

TEST(ramfs, can_lookup_for_file_in_root_dir, ramfs_fixture) {
    auto root_node = fs.lookup("/", {});
    REQUIRE_FALSE(fs.lookup("/file1", {}));
    fs.create("file1", root_node, vfs::vnode::type::file);
    fs.create("file2", root_node, vfs::vnode::type::file);
    fs.create("file3", root_node, vfs::vnode::type::file);
    REQUIRE(fs.lookup("/file1", {}));
    REQUIRE(fs.lookup("/file2", {}));
    REQUIRE(fs.lookup("/file3", {}));
    REQUIRE_FALSE(fs.lookup("/file4", {}));
}

TEST(ramfs, can_create_file_in_dir, ramfs_fixture) {
    auto root_node = fs.lookup("/", {});
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    auto file_node = fs.create("file", dir_node, vfs::vnode::type::file);
    REQUIRE(file_node);
    REQUIRE(file_node->node_type == vfs::vnode::type::file);
}

TEST(ramfs, can_lookup_for_file_in_dir, ramfs_fixture) {
    auto root_node = fs.lookup("/", {});
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    REQUIRE_FALSE(fs.lookup("/dir/file", {}));
    fs.create("file", dir_node, vfs::vnode::type::file);
    REQUIRE(fs.lookup("/dir", {}));
    REQUIRE(fs.lookup("/dir/file", {}));
    REQUIRE_FALSE(fs.lookup("/dir/file2", {}));
    REQUIRE_FALSE(fs.lookup("/dir/file/file", {}));
}

