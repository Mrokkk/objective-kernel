#pragma once

class heap_allocator {

    char *_heap = nullptr;

public:

    explicit heap_allocator(char *heap_start)
        : _heap(heap_start) {
    }

    void *grow_heap(unsigned long value) {
        auto prev_heap = _heap;
        _heap += value;
        return prev_heap;
    }

};

