#include <cstdint>
#include <kernel/definitions.hpp>
#include <kernel/cpu/common.hpp>
#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/cache.hpp>

#include <yatf.hpp>

TEST(ramfs, can_create) {
    ramfs::ramfs fs;
    REQUIRE_EQ((const char *)fs.name(), "ramfs");
    auto root_node = fs.lookup("/", nullptr);
    REQUIRE(root_node);
    REQUIRE_EQ(root_node->id, 1u);
}

TEST(ramfs, parent_node_has_to_be_passed_to_create) {
    ramfs::ramfs fs;
    REQUIRE_FALSE(fs.create("file", nullptr, vfs::vnode::type::file));
    REQUIRE_FALSE(fs.create("dir", nullptr, vfs::vnode::type::dir));
}

TEST(ramfs, can_create_dir) {
    ramfs::ramfs fs;
    auto root_node = fs.lookup("/", nullptr);
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    REQUIRE(dir_node);
    REQUIRE(dir_node->node_type == vfs::vnode::type::dir);
}

TEST(ramfs, can_create_file) {
    ramfs::ramfs fs;
    auto root_node = fs.lookup("/", nullptr);
    auto dir_node = fs.create("file", root_node, vfs::vnode::type::file);
    REQUIRE(dir_node);
    REQUIRE(dir_node->node_type == vfs::vnode::type::file);
}

TEST(ramfs, can_lookup_for_file_in_root_dir) {
    ramfs::ramfs fs;
    auto root_node = fs.lookup("/", nullptr);
    REQUIRE_FALSE(fs.lookup("/file1", nullptr));
    fs.create("file1", root_node, vfs::vnode::type::file);
    fs.create("file2", root_node, vfs::vnode::type::file);
    fs.create("file3", root_node, vfs::vnode::type::file);
    REQUIRE(fs.lookup("/file1", nullptr));
    REQUIRE(fs.lookup("/file2", nullptr));
    REQUIRE(fs.lookup("/file3", nullptr));
    REQUIRE_FALSE(fs.lookup("/file4", nullptr));
}

TEST(ramfs, can_create_file_in_dir) {
    ramfs::ramfs fs;
    auto root_node = fs.lookup("/", nullptr);
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    auto file_node = fs.create("file", dir_node, vfs::vnode::type::file);
    REQUIRE(file_node);
    REQUIRE(file_node->node_type == vfs::vnode::type::file);
}

TEST(ramfs, can_lookup_for_file_in_dir) {
    ramfs::ramfs fs;
    auto root_node = fs.lookup("/", nullptr);
    auto dir_node = fs.create("dir", root_node, vfs::vnode::type::dir);
    REQUIRE_FALSE(fs.lookup("/dir/file", nullptr));
    fs.create("file", dir_node, vfs::vnode::type::file);
    REQUIRE(fs.lookup("/dir", nullptr));
    REQUIRE(fs.lookup("/dir/file", nullptr));
    REQUIRE_FALSE(fs.lookup("/dir/file2", nullptr));
    REQUIRE_FALSE(fs.lookup("/dir/file/file", nullptr));
}

