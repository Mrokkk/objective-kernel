#pragma once

namespace memory {

namespace paging {

class frames_allocator {

    uint32_t *data_ = nullptr;
    size_t size_ = 0u;

public:

    class pointer {

        uint32_t *ptr_ = nullptr;
        uint32_t bit_ = 0;

    public:

        pointer() = default;

        explicit pointer(uint32_t *ptr, uint32_t bit) : ptr_(ptr), bit_(bit) {
        }

        operator bool() const {
            return *ptr_ & (1 << bit_);
        }

        bool operator=(bool v) {
            if (v) {
                *ptr_ |= 1 << bit_;
            }
            else {
                *ptr_ &= ~(1 << bit_);
            }
            return true;
        }

    };

    frames_allocator() = default;

    template <typename Pointer>
    void set(Pointer ptr, uint32_t available_mem, uint32_t memory_to_cover) {
        data_ = static_cast<uint32_t *>(ptr);
        size_ = available_mem / 0x1000;
        auto to_set = memory_to_cover / 0x1000;
        auto count = to_set / 32;
        auto bits = to_set % 32;
        for (auto i = 0u; i < count; i++) {
            data_[i] = ~0UL;
        }
        data_[count] = (~0UL >> (32 - bits));
    }

    bool operator[](size_t n) const {
        return !(data_[n / 32] & (1 << (n % 32)));
    }

    bool is_free(uint32_t address) const {
        uint32_t frame = address / 4096;
        return !(data_[frame / 32] & (1 << (frame % 32)));
    }

    uint32_t allocate() {
        auto i = 0u;
        for (i = 0; i < size_ * 32; i++) {
            if (is_free(i * 0x1000)) break;
        }
        data_[i / 32] |= (1 << (i % 32));
        return i * 0x1000;
    }

};

extern frames_allocator frames;

} // namespace paging

} // namespace memory

