#pragma once

#include <list.hpp>
#include <string.hpp>

#include "vnode.hpp"
#include "definitions.hpp"
#include "file_system.hpp"

#include <kernel/console/logger.hpp>

namespace ramfs {

class ramfs final : public vfs::file_system {

    unsigned node_nr = 2;

    class file_content {

        char *data_ = nullptr;
        char *iterator_ = nullptr;

        void allocate(size_t size) {
            data_ = new char[size];
            iterator_ = data_;
        }

        void reallocate(size_t new_size) {
            auto new_data = new char[new_size];
            auto size = data_written();
            utils::memcopy(data_, new_data, size);
            delete [] data_;
            data_ = new_data;
            iterator_ = data_ + size;
        }

    public:

        file_content() = default;

        ~file_content() {
            if (data_) {
                delete [] data_;
            }
        }

        size_t data_written() const {
            return iterator_ - data_;
        }

        void write(size_t pos, const char *data, size_t size) {
            if (data_ == nullptr) {
                allocate(pos + size);
                utils::fill(data_, pos + size, 0);
            }
            if (size + pos > data_written()) {
                reallocate(data_written() + pos + size);
                iterator_ = data_ + size + pos;
            }
            utils::memcopy(data, data_ + pos, size);
        }

        void read(size_t pos, char *data, size_t size) {
            if (data_ == nullptr) {
                return;
            }
            utils::memcopy(data_ + pos, data, size);
        }

    };

    struct dir_entry {

        unsigned id = 0;
        utils::string file_name;
        vfs::vnode::type file_type;
        file_content content;
        utils::list<dir_entry *> dir_entries;

        dir_entry(unsigned id, const utils::string &name, vfs::vnode::type t)
                : id(id), file_name(name), file_type(t) {
        }

        ~dir_entry() {
            for (auto &entry : dir_entries) {
                delete entry;
            }
        }

    };

    dir_entry root_;

    dir_entry *lookup_in_dir(utils::list<dir_entry *> &dir, const utils::string &name);
    dir_entry *dir_entry_lookup(const vfs::path_t &path);

public:

    ramfs() : root_(1, "/", vfs::vnode::type::dir) {
    }

    utils::string name() override;
    vfs::vnode_t lookup(const vfs::path_t &path, vfs::vnode_t parent) override;
    vfs::vnode_t create(const vfs::path_t &path, vfs::vnode_t parent, vfs::vnode::type type) override;
    int read(vfs::file *file, vfs::vnode_t &vnode, char *buffer, size_t size = 0) override;
    int write(vfs::file *file, vfs::vnode_t &vnode, const char *buffer, size_t size) override;
    void sync(vfs::vnode &vnode) override;

};

} // namespace ramfs

