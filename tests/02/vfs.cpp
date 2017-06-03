#include <kernel/vfs/vfs.hpp>
#include <kernel/vfs/file.hpp>
#include <kernel/vfs/ramfs.hpp>
#include <kernel/vfs/cache.hpp>

#include <yatf.hpp>

namespace test_cases {

TEST(vfs, can_create_root) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto node = vfs.lookup("/");
    REQUIRE(node);
    REQUIRE_EQ(node->id, 1u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    REQUIRE_EQ(node->size, 0u);
}

TEST(vfs, can_create_files) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    REQUIRE(vfs.create("/some_file", vfs::vnode::type::file));
    {
        auto node2 = vfs.lookup("/some_file");
        REQUIRE_EQ(node2->id, 2u);
        REQUIRE_EQ(node2->size, 0u);
        REQUIRE(node2->node_type == vfs::vnode::type::file);
    }
    auto node = vfs.create("/some_file2", vfs::vnode::type::file);
    REQUIRE_EQ(node->mount_point->fs, &ramfs);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 3u);
    REQUIRE_EQ(node->size, 0u);
    REQUIRE(node->node_type == vfs::vnode::type::file);
    {
        auto node2 = vfs.lookup("/some_file2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE_EQ(node2->size, 0u);
        REQUIRE(node2->node_type == vfs::vnode::type::file);
    }
}

TEST(vfs, can_create_dirs) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto node = vfs.create("/some_dir", vfs::vnode::type::dir);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 2u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    {
        auto node2 = vfs.lookup("/some_dir");
        REQUIRE_EQ(node2->id, 2u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
    node = vfs.create("/some_dir2", vfs::vnode::type::dir);
    REQUIRE(node);
    REQUIRE_EQ(node->id, 3u);
    REQUIRE(node->node_type == vfs::vnode::type::dir);
    {
        auto node2 = vfs.lookup("/some_dir2");
        REQUIRE_EQ(node2->id, 3u);
        REQUIRE(node2->node_type == vfs::vnode::type::dir);
        REQUIRE_EQ(node2->size, 0u);
    }
}

TEST(vfs, cannot_lookup_for_nonexistent_path) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    REQUIRE_FALSE(vfs.lookup("/a"));
    REQUIRE_FALSE(vfs.lookup("/dir1"));
    REQUIRE_FALSE(vfs.lookup("/dir2"));
    REQUIRE_FALSE(vfs.lookup("/file"));
    REQUIRE_FALSE(vfs.lookup("/file/dir"));
    REQUIRE_FALSE(vfs.lookup("/file/dir/a/b"));
}

TEST(vfs, can_create_tree) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto node2 = vfs.create("/some_dir", vfs::vnode::type::dir);
    REQUIRE(node2);
    auto node3 = vfs.create("/file", vfs::vnode::type::file);
    REQUIRE(node3);
    auto node4 = vfs.create("/some_dir/file1", vfs::vnode::type::file);
    REQUIRE(node4);
    auto node5 = vfs.create("/some_dir/file2", vfs::vnode::type::file);
    REQUIRE(node5);
    auto node6 = vfs.create("/some_dir/dir1", vfs::vnode::type::dir);
    REQUIRE(node6);
    auto node7 = vfs.create("/some_dir/dir1/file", vfs::vnode::type::file);
    REQUIRE(node7);
    {
        auto node = vfs.lookup("/file");
        REQUIRE(node);
        REQUIRE(node == node3);
        REQUIRE_EQ(node->id, 3u);
        REQUIRE(node->node_type == vfs::vnode::type::file);
    }
    {
        auto node = vfs.lookup("/some_dir/file1");
        REQUIRE(node);
        REQUIRE(node == node4);
        REQUIRE_EQ(node->id, 4u);
        REQUIRE(node->node_type == vfs::vnode::type::file);
    }
    {
        auto node = vfs.lookup("/some_dir/dir1/file");
        REQUIRE(node);
        REQUIRE(node == node7);
        REQUIRE_EQ(node->id, 7u);
        REQUIRE(node->node_type == vfs::vnode::type::file);
    }
    {
        auto node = vfs.lookup("/some_dir/dir1");
        REQUIRE(node);
        REQUIRE(node == node6);
        REQUIRE_EQ(node->id, 6u);
        REQUIRE(node->node_type == vfs::vnode::type::dir);
    }
    {
        auto node = vfs.lookup("/some_dir/file2");
        REQUIRE(node);
        REQUIRE(node == node5);
        REQUIRE_EQ(node->id, 5u);
        REQUIRE(node->node_type == vfs::vnode::type::file);
    }
}

TEST(vfs, can_cache_nodes) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    {
        auto orig_node = vfs.create("/some_file", vfs::vnode::type::file);
        auto cache_entry = vfs.get_cache().find(*orig_node);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_file");
    }
    {
        auto orig_node = vfs.create("/some_dir", vfs::vnode::type::dir);
        auto cache_entry = vfs.get_cache().find(*orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_dir");
        REQUIRE(cache_entry->node->node_type == vfs::vnode::type::dir);
    }
    {
        auto orig_node = vfs.create("/some_dir/some_other_file", vfs::vnode::type::file);
        auto cache_entry = vfs.get_cache().find(*orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(cache_entry->node == orig_node);
        REQUIRE(cache_entry->name == "some_other_file");
    }
    {
        auto orig_node = vfs.create("/some_dir/other_file", vfs::vnode::type::file);
        auto node = vfs.lookup("/some_dir");
        auto cache_entry = vfs.get_cache().find(*orig_node);
        REQUIRE(cache_entry != nullptr);
        REQUIRE(node->node_type == vfs::vnode::type::dir);
    }
    {
        auto node = vfs.lookup("/some_dir");
        REQUIRE(node);
    }
}

TEST(vfs, open_can_create_file) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto file = vfs.open("/file", vfs::file::mode::write);
    REQUIRE(file);
    REQUIRE_EQ(file->size(), 0u);
    auto node = vfs.lookup("/file");
    REQUIRE(node);
    REQUIRE_EQ(node->size, 0u);
    REQUIRE_EQ(node->id, 2u);
}

TEST(vfs, cannot_open_directory) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    vfs.create("/some_dir", vfs::vnode::type::dir);
    auto file = vfs.open("/some_dir", vfs::file::mode::read);
    REQUIRE_FALSE(file);
    file = vfs.open("/some_dir", vfs::file::mode::write);
    REQUIRE_FALSE(file);
    file = vfs.open("/some_dir", vfs::file::mode::read_write);
    REQUIRE_FALSE(file);
}

TEST(vfs, can_read_write_to_file) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto file = vfs.open("/file", vfs::file::mode::read_write);
    REQUIRE(file);
    REQUIRE_EQ(file->position(), 0);
    file->write("some text", 10);
    REQUIRE_EQ(file->size(), 10u);
    REQUIRE_EQ(file->position(), 10);
    file->seek(0u);
    REQUIRE_EQ(file->position(), 0);
    char buffer[32];
    file->read(buffer, 10);
    REQUIRE_EQ(file->position(), 10);
    REQUIRE_EQ((const char *)buffer, "some text");
    file->seek(5);
    REQUIRE_EQ(file->position(), 5);
    file->read(buffer, 5);
    REQUIRE_EQ(file->position(), 10);
    REQUIRE_EQ((const char *)buffer, "text");
    file->seek(5);
    file->write("testing", 8);
    REQUIRE_EQ(file->size(), 13u);
    file->seek(0);
    REQUIRE_EQ(file->position(), 0);
    file->read(buffer, 13);
    REQUIRE_EQ((const char *)buffer, "some testing");
    REQUIRE_EQ(file->position(), 13);
    file->seek(12u);
    file->write(" for kernel", 12u);
    file->seek(0);
    file->read(buffer, 24u);
    REQUIRE_EQ((const char *)buffer, "some testing for kernel");
    REQUIRE_EQ(file->size(), 24u);
}

TEST(vfs, cannot_create_file_under_file) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto file_node = vfs.create("/some_file", vfs::vnode::type::file);
    {
        auto bad_node = vfs.create("/some_file/file", vfs::vnode::type::file);
        REQUIRE_FALSE(bad_node);
    }
    {
        auto bad_node = vfs.create("/some_file/dir", vfs::vnode::type::dir);
        REQUIRE_FALSE(bad_node);
    }
    {
        auto bad_node = vfs.create("/some_file/dir/dir", vfs::vnode::type::dir);
        REQUIRE_FALSE(bad_node);
    }
}

TEST(vfs, cannot_create_files_with_the_same_name) {
    ramfs::ramfs ramfs;
    vfs::vfs vfs(ramfs);
    auto file_node = vfs.create("/some_file", vfs::vnode::type::file);
    for (auto i = 0; i < 32; ++i) {
        auto bad_node = vfs.create("/some_file", vfs::vnode::type::file);
        REQUIRE_FALSE(bad_node);
    }
}

} // namespace test_cases

