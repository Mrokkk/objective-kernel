#pragma once

namespace vfs {

using dev_t = short;

struct block_device {

    struct request {
        enum class operation {
            read,
            write
        };
        unsigned block;
        unsigned size;
        char *buffer;
        int error;
    };

    virtual const char *name() = 0;
    virtual void handle_request(request *req) = 0;

};

struct null_block_device : public block_device {

    const char *name() override {
        return "null";
    }

    void handle_request(block_device::request *) override {
    }

};

extern null_block_device null_bd_;

} // namespace vfs

