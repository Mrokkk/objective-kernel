#pragma once

#include <bitset.hpp>
#include "memory.hpp"

namespace memory {

struct physical_memory_manager {

    physical_memory_manager(uint32_t available_mem, uint32_t memory_to_cover);
    uint32_t allocate();
    void deallocate(uint32_t address);
    uint32_t end() const;

private:
    size_t size_;
    uint32_t end_;
    utils::bitset<32768> data_;
};

extern physical_memory_manager frames;

} // namespace memory

