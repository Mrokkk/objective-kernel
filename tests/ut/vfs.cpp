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

TEST(cache, new_cache_is_empty) {
    vfs::cache c;
    REQUIRE(c.empty());
    REQUIRE_FALSE(c.find("/", nullptr));
}

TEST(cache, can_add_root_element) {
    vfs::cache c;
    auto root_node = utils::make_shared<vfs::vnode>(1u, 0u, 0u, nullptr, nullptr, vfs::vnode::type::dir);
    c.add("/", root_node, nullptr);
    auto cached_node = c.find("/")->node;
    REQUIRE(cached_node);
    REQUIRE(cached_node == root_node);
    REQUIRE_EQ(cached_node->id, 1u);
    REQUIRE_EQ(cached_node->fs, nullptr);
    REQUIRE_EQ(cached_node->data, nullptr);
}

