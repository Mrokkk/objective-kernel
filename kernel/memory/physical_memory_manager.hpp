#pragma once

#include <bitset.hpp>
#include "memory.hpp"

namespace memory {

struct physical_memory_manager {

    struct frame {
        size_t index;
        uint32_t address;
    };

    physical_memory_manager(uint32_t available_mem, uint32_t memory_to_cover);
    frame allocate_frame();
    void deallocate_frame(uint32_t address);
    uint32_t end() const;

private:
    uint32_t end_;
    size_t size_;
    utils::bitset<32768> data_;
};

} // namespace memory

