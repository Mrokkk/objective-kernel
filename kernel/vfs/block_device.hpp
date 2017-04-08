#pragma once

namespace vfs {

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

} // namespace vfs

