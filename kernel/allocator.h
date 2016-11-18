#pragma once

#include <stddef.h>
#include <af_list.h>

namespace kernel {

template <class Heap_Allocator, unsigned short _memory_block_size = 16>
class allocator {

    union memory_block {
        struct {
            size_t size;
            char free;
            yacppl::af_list::list_head blocks;
        } control;
        struct {
            unsigned char dummy[_memory_block_size];
            unsigned int block_ptr[0];
        } data;
        memory_block(size_t size) {
            control.size = size;
            control.free = 0;
        }
    } __attribute__((packed));

    yacppl::af_list::list_head _blocks;
    Heap_Allocator _heap_allocator;

    memory_block *create_memory_block(size_t size) {
        return new(_heap_allocator.grow_heap(_memory_block_size + size)) memory_block(size);
    }

public:

    explicit allocator(char *heap_start)
        : _heap_allocator(heap_start) {}

    void *allocate(size_t size) {
        if (size % _memory_block_size)
            size = (size / _memory_block_size) * _memory_block_size + _memory_block_size;
        memory_block *new_block = nullptr;
        memory_block *temp = nullptr;
        list_for_each_entry(temp, &_blocks, control.blocks) {
            if (temp->control.free && temp->control.size >= size) {
                auto old_size = temp->control.size;
                if (old_size <= size + 2 * _memory_block_size) {
                    temp->control.free = 0;
                }
                else {
                    temp = new(temp) memory_block(size);
                    new_block = reinterpret_cast<memory_block *>(reinterpret_cast<size_t>(temp->data.block_ptr) + size);
                    new_block->control.size = old_size - _memory_block_size - temp->control.size;
                    new_block->control.free = 1;
                    list_add(&new_block->control.blocks, &temp->control.blocks);
                }
                return static_cast<void *>(temp->data.block_ptr);
            }
        }
        if (!(new_block = create_memory_block(size))) return 0;
        list_add_tail(&new_block->control.blocks, &_blocks);
        return static_cast<void *>(new_block->data.block_ptr);
    }

    int free(void *address) {
        memory_block *temp;
        list_for_each_entry(temp, &_blocks, control.blocks) {
            if ((unsigned int) temp->data.block_ptr == (unsigned int) address) {
                temp->control.free = 1;
                return 0;
            }
            if (temp->control.blocks.next != &_blocks) {
                auto next = list_entry(temp->control.blocks.next, memory_block, control.blocks);
                if (next->control.free && temp->control.free) {
                    temp->control.size = temp->control.size + next->control.size + _memory_block_size;
                    list_del(&next->control.blocks);
                }
            }
        }
        return -1;
    }

    size_t get_size() {
        return sizeof(memory_block);
    }

};

} // namespace kernel

