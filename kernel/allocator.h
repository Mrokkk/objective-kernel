#pragma once

#include <af_list.h>

namespace kernel {

template <unsigned short _memory_block_size = 16>
class allocator {

    struct memory_block {
        union {
            struct {
                unsigned int size;
                char free;
                yacppl::af_list::list_head blocks;
            } control;
            struct {
                unsigned char dummy[_memory_block_size];
                unsigned int block_ptr[0];
            } data;
        };
        memory_block(unsigned int size) {
            control.size = size;
            control.free = 0;
        }
    } __attribute__((packed));

    yacppl::af_list::list_head _blocks;
    char *_heap = nullptr;

    void *grow_heap(unsigned long value) {
        auto prev_heap = _heap;
        _heap += value;
        return prev_heap;
    }

    memory_block *create_memory_block(unsigned int size) {
        return new(grow_heap(_memory_block_size + size)) memory_block(size);
    }

public:

    explicit allocator(char *heap_start)
        : _heap(heap_start) {}

    void *allocate(unsigned long size) {
        if (size % _memory_block_size)
            size = (size / _memory_block_size) * _memory_block_size + _memory_block_size;
        memory_block *new_block = nullptr;
        memory_block *temp = nullptr;
        list_for_each_entry(temp, &_blocks, control.blocks) {
            if (temp->control.free && temp->control.size >= size) {
                unsigned int old_size = temp->control.size;
                if (old_size <= size + 2 * _memory_block_size) {
                    temp->control.free = 0;
                }
                else {
                    temp = new(temp) memory_block(size);
                    new_block = (memory_block *)
                        ((unsigned int) temp->data.block_ptr + size);
                    new_block->control.size = old_size - _memory_block_size - temp->control.size;
                    new_block->control.free = 1;
                    list_add(&new_block->control.blocks, &temp->control.blocks);
                }
                return (void *) temp->data.block_ptr;
            }
        }
        if (!(new_block = create_memory_block(size))) return 0;
        list_add_tail(&new_block->control.blocks, &_blocks);
        return (void *) new_block->data.block_ptr;
    }

    int free(void *address) {
        memory_block *temp;
        list_for_each_entry(temp, &_blocks, control.blocks) {
            if ((unsigned int) temp->data.block_ptr == (unsigned int) address) {
                temp->control.free = 1;
                return 0;
            }
            if (temp->control.blocks.next != &_blocks) {
                memory_block *next =
                    list_entry(temp->control.blocks.next, memory_block, control.blocks);
                if (next->control.free && temp->control.free) {
                    temp->control.size = temp->control.size + next->control.size + _memory_block_size;
                    list_del(&next->control.blocks);
                }
            }
        }
        return -1;
    }

};

} // namespace kernel

