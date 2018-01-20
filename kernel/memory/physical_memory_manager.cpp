#include "physical_memory_manager.hpp"

namespace memory {

physical_memory_manager::physical_memory_manager(uint32_t available_mem, uint32_t memory_to_cover)
        : end_(memory_to_cover)
        , size_(frame_number(available_mem)) {
    const auto count = frame_number(memory_to_cover);
    for (auto i = 0u; i < count; ++i) {
        data_[i] = true;
    }
}

physical_memory_manager::frame physical_memory_manager::allocate_frame() {
    uint32_t i;
    for (i = 0u; i < size_; ++i) {
        if (!data_[i]) break;
    }
    data_[i] = true;
    return frame{i, frame_address(i)};
}

void physical_memory_manager::deallocate_frame(uint32_t address) {
    data_[frame_number(address)] = false;
}

uint32_t physical_memory_manager::end() const {
    return end_;
}

} // namespace memory

