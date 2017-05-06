#pragma once

#include <bitset.h>
#include "paging.hpp"

namespace memory {

namespace paging {

class frames_allocator {

    size_t size_;
    utils::bitset<32768> data_;

public:

    frames_allocator() = default;

    void set(uint32_t available_mem, uint32_t memory_to_cover) {
        size_ = frame_number(available_mem);
        auto count = frame_number(memory_to_cover);
        for (auto i = 0u; i < count; ++i) {
            data_[i] = true;
        }
    }

    uint32_t allocate() {
        uint32_t i;
        for (i = 0u; i < size_; ++i) {
            if (!data_[i]) break;
        }
        data_[i] = true;
        return frame_address(i);
    }

};

extern frames_allocator frames;

} // namespace paging

} // namespace memory

