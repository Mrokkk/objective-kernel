#pragma once

#include <stddef.h>

namespace kernel {

class heap_allocator {

    char *_heap = nullptr;

public:

    explicit heap_allocator(char *heap_start)
        : _heap(heap_start) {
    }

    void *grow_heap(size_t value) {
        auto prev_heap = _heap;
        _heap += value;
        return prev_heap;
    }

};

} // namespace kernel

