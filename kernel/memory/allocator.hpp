#pragma once

#include <kernel_list.hpp>
#include <lazy_instance.hpp>
#include <spinlock.hpp>
#include "virtual_memory_manager.hpp"

namespace memory {

template <class BackendAllocator, size_t _memory_block_size>
class allocator final {

    class memory_block final {

        struct _data {
            uint8_t data[_memory_block_size];
            uint8_t block_ptr[0];
        } __attribute__((packed));

        template <typename Ptr>
        Ptr pointer_offset(Ptr ptr, int off) {
            return reinterpret_cast<Ptr>(reinterpret_cast<char *>(ptr) + off);
        }

    public:

        typename utils::kernel_list<memory_block>::node list_;
        uint32_t size;
        bool free = false;

        constexpr explicit memory_block(size_t s)
            : size(s), free(false) {
        }

        void divide(size_t pivot) {
            auto old_size = size;
            size = pivot;
            free = false;
            auto new_block = reinterpret_cast<memory_block *>(pointer_offset(data(), pivot));
            new_block->size = old_size - _memory_block_size - size;
            new_block->free = true;
            list_.insert(&new_block->list_);
        }

        void try_to_divide(size_t pivot) {
            if (size <= pivot + 2 * _memory_block_size)
                free = false;
            else
                divide(pivot);
        }

        void *data() {
            return reinterpret_cast<_data *>(this)->block_ptr;
        }

        constexpr void *operator new(size_t, void *address) {
            return address;
        }

    };

    utils::kernel_list<memory_block> blocks_;
    BackendAllocator *backend_allocator_;
    static utils::spinlock spinlock_;
    uint32_t heap_ = 0;
    uint32_t end_ = 0;

    void adapt_size(size_t &size) const {
        if (size % _memory_block_size)
            size = (size / _memory_block_size) * _memory_block_size + _memory_block_size;
    }

    void grow_heap() {
        heap_ = (uint32_t)backend_allocator_->allocate();
        end_ = heap_ + 4096;
    }

    memory_block *create_memory_block(size_t size) {
        if (heap_ + _memory_block_size + size >= end_) {
            grow_heap();
        }
        auto block = new((void *)heap_) memory_block(size);
        heap_ += _memory_block_size + size;
        return block;
    }

public:

    explicit allocator(BackendAllocator* backend_allocator)
            : blocks_(&memory_block::list_)
            , backend_allocator_(backend_allocator) {
        grow_heap();
    }

    void *allocate(size_t size) {
        adapt_size(size);
        auto _ = make_scoped_lock(spinlock_);
        for (auto &temp : blocks_) {
            if (temp.free && temp.size >= size) {
                temp.try_to_divide(size);
                return temp.data();
            }
        }
        auto new_block = create_memory_block(size);
        if (new_block == nullptr) return nullptr;
        blocks_.push_back(*new_block);
        return new_block->data();
    }

    bool free(void *address) {
        auto _ = make_scoped_lock(spinlock_);
        for (auto &temp : blocks_) {
            if (temp.data() == address) {
                temp.free = true;
                return true;
            }
            auto next = temp.list_.next()->entry();
            if (!next) break;
            //if (next->free && temp.free) {
                //temp.size = temp.size + next->size + _memory_block_size;
                //blocks_.erase(*next);
            //}
        }
        return false;
    }

};

template <class BackendAllocator, size_t _memory_block_size>
utils::spinlock allocator<BackendAllocator, _memory_block_size>::spinlock_;

constexpr const size_t memory_block_size = 32;
extern utils::lazy_instance<allocator<virtual_memory_manager, memory_block_size>> heap_allocator;

} // namespace memory

