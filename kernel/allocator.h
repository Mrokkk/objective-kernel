#pragma once

#include <af_list.h>

// TODO: move to something like <new.h>
void *operator new(size_t, void *address);

namespace kernel {

template <class Heap_Allocator, size_t _memory_block_size>
class allocator final {

    class memory_block {

        struct _data {
            unsigned char data[_memory_block_size];
            unsigned int block_ptr[0];
        } __packed;

    public:

        size_t size;
        bool free = false;
        yacppl::af_list<memory_block> blocks;

        explicit memory_block(size_t s)
            : size(s), free(false), blocks(&memory_block::blocks) {
        }

        void *data() {
            return reinterpret_cast<_data *>(this)->block_ptr;
        }

    };

    yacppl::af_list<memory_block> _blocks;
    Heap_Allocator _heap_allocator;

    memory_block *create_memory_block(size_t size) {
        return new(_heap_allocator.grow_heap(_memory_block_size + size)) memory_block(size);
    }

    template <typename Ptr>
    Ptr pointer_offset(Ptr ptr, int off) {
        return reinterpret_cast<Ptr>(reinterpret_cast<char *>(ptr) + off);
    }

public:

    explicit allocator(char *heap_start)
        : _blocks(&memory_block::blocks), _heap_allocator(heap_start) {}

    void *allocate(size_t size) {
        if (size % _memory_block_size)
            size = (size / _memory_block_size) * _memory_block_size + _memory_block_size;
        memory_block *new_block = nullptr;
        for (auto &temp : _blocks) {
            if (temp.free && temp.size >= size) {
                auto old_size = temp.size;
                if (old_size <= size + 2 * _memory_block_size) {
                    temp.free = false;
                }
                else {
                    auto block = &temp;
                    block->size = size;
                    block->free = false;
                    new_block = reinterpret_cast<memory_block *>(pointer_offset(block->data(), block->size));
                    new_block->size = old_size - _memory_block_size - size;
                    new_block->free = true;
                    temp.blocks.add_front(&new_block->blocks);
                }
                return temp.data();
            }
        }
        if (!(new_block = create_memory_block(size))) return 0;
        _blocks.add(&new_block->blocks);
        return new_block->data();
    }

    int free(void *address) {
        for (auto &temp : _blocks) {
            if (temp.data() == address) {
                temp.free = true;
                return 0;
            }
            auto next = temp.blocks.next_entry();
            if (!next) continue;
            if (next->free && temp.free) {
                temp.size = temp.size + next->size + _memory_block_size;
                next->blocks.remove();
            }
        }
        return -1;
    }

};

} // namespace kernel

