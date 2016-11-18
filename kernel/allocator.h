#pragma once

#include <af_list.h>
#include <basic/definitions.h>

namespace kernel {

template <class Heap_Allocator, size_t _memory_block_size>
class allocator final {

    union memory_block {

        struct {
            size_t size;
            char free;
            yacppl::af_list::list_head blocks;
        } control;

        struct {
            unsigned char dummy[_memory_block_size];
            unsigned int block_ptr[0];
        } data;

        explicit memory_block(size_t size) {
            control.size = size;
            control.free = 0;
        }

    } __packed;

    yacppl::af_list::list_head _blocks;
    Heap_Allocator _heap_allocator;

    memory_block *create_memory_block(size_t size);

public:

    explicit allocator(char *heap_start)
        : _heap_allocator(heap_start) {}

    void *allocate(size_t size);

    int free(void *address);

};

} // namespace kernel

