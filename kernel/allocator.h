#pragma once

#include <af_list.h>

namespace kernel {

template <class Heap_Allocator, size_t _memory_block_size>
class allocator final {

    struct memory_block {
        size_t size;
        bool free;
        yacppl::af_list<memory_block> blocks;
        explicit memory_block(size_t s)
            : size(s), free(false), blocks(&memory_block::blocks) {
        }
    };

    struct memory_block_data {
        unsigned char dummy[_memory_block_size];
        unsigned int block_ptr[0];
    } __packed;

    yacppl::af_list<memory_block> _blocks;
    Heap_Allocator _heap_allocator;

    memory_block *create_memory_block(size_t size);

public:

    explicit allocator(char *heap_start)
        : _blocks(&memory_block::blocks), _heap_allocator(heap_start) {}

    void *allocate(size_t size);

    int free(void *address);

};

} // namespace kernel

