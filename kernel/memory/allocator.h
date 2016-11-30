#pragma once

#include <inherited_list.h>
#include <basic/new.h>

namespace memory {

template <class Heap_Allocator, size_t _memory_block_size>
class allocator final {

    class memory_block : public yacppl::inherited_list<memory_block> {

        struct _data {
            unsigned char data[_memory_block_size];
            unsigned int block_ptr[0];
        } __packed;

        template <typename Ptr>
        Ptr pointer_offset(Ptr ptr, int off) {
            return reinterpret_cast<Ptr>(reinterpret_cast<char *>(ptr) + off);
        }

    public:

        size_t size;
        bool free = false;

        explicit memory_block(size_t s)
            : size(s), free(false) {
        }

        void divide(size_t pivot) {
            auto old_size = size;
            size = pivot;
            free = false;
            auto new_block = reinterpret_cast<memory_block *>(pointer_offset(data(), pivot));
            new_block->size = old_size - _memory_block_size - size;
            new_block->free = true;
            yacppl::inherited_list<memory_block>::add_front(new_block);
        }

        void try_to_divide(size_t pivot) {
            if (size <= pivot + 2 * _memory_block_size) {
                free = false;
            }
            else {
                divide(pivot);
            }
        }

        void *data() {
            return reinterpret_cast<_data *>(this)->block_ptr;
        }

    };

    yacppl::inherited_list<memory_block> _blocks;
    Heap_Allocator _heap_allocator;

    void adapt_size(size_t &size) {
        if (size % _memory_block_size)
            size = (size / _memory_block_size) * _memory_block_size + _memory_block_size;
    }

    memory_block *create_memory_block(size_t size) {
        return new(_heap_allocator.grow_heap(_memory_block_size + size)) memory_block(size);
    }

public:

    explicit allocator(char *heap_start)
        : _heap_allocator(heap_start) {}

    void *allocate(size_t size) {
        adapt_size(size);
        for (auto &temp : _blocks) {
            if (temp.free && temp.size >= size) {
                temp.try_to_divide(size);
                return temp.data();
            }
        }
        auto new_block = create_memory_block(size);
        if (new_block == nullptr) return 0;
        _blocks.add(new_block);
        return new_block->data();
    }

    int free(void *address) {
        for (auto &temp : _blocks) {
            if (temp.data() == address) {
                temp.free = true;
                return 0;
            }
            auto next = temp.next();
            if (!next) continue;
            if (next->free && temp.free) {
                temp.size = temp.size + next->size + _memory_block_size;
                next->remove();
            }
        }
        return -1;
    }

};

} // namespace memory

