#pragma once

namespace vfs {

struct IBlockDevice {

    struct Request {
        enum class Operation {
            Read,
            Write
        };
        unsigned block;
        unsigned size;
        char *buffer;
        int error;
    };

    virtual const char *name() = 0;
    virtual void handle_request(Request *request) = 0;

};

} // namespace vfs

