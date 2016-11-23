#include <kernel/allocator.h>
#include <kernel/heap_allocator.h>

asmlinkage char _end[];

namespace kernel {

static allocator<heap_allocator, 32> a(_end);

#define _Memory_block typename allocator<Heap_Allocator, _memory_block_size>::memory_block
#define _Allocator allocator<Heap_Allocator, _memory_block_size>

template <class Heap_Allocator, size_t _memory_block_size>
_Memory_block *_Allocator::create_memory_block(size_t size) {
    return new(_heap_allocator.grow_heap(_memory_block_size + size)) memory_block(size);
}

template <class Heap_Allocator, size_t _memory_block_size>
void *_Allocator::allocate(size_t size) {
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
                auto block = new(&temp) memory_block(size);
                new_block = reinterpret_cast<memory_block *>(reinterpret_cast<size_t>(reinterpret_cast<memory_block_data *>(block)->block_ptr) + size);
                new_block->size = old_size - _memory_block_size - temp.size;
                new_block->free = true;
                temp.blocks.add_front(&new_block->blocks);
            }
            return static_cast<void *>(reinterpret_cast<memory_block_data *>(&temp)->block_ptr);
        }
    }
    if (!(new_block = create_memory_block(size))) return 0;
    _blocks.add(&new_block->blocks);
    return static_cast<void *>(reinterpret_cast<memory_block_data *>(new_block)->block_ptr);
}

template <class Heap_Allocator, size_t _memory_block_size>
int _Allocator::free(void *address) {
    for (auto &temp : _blocks) {
        if (reinterpret_cast<unsigned long>(reinterpret_cast<memory_block_data *>(&temp)->block_ptr) == reinterpret_cast<unsigned long>(address)) {
            temp.free = true;
            return 0;
        }
        //if (temp.blocks.next != &_blocks) {
        auto next = temp.blocks.next_entry();
        if (next->free && temp.free) {
            temp.size = temp.size + next->size + _memory_block_size;
            next->blocks.remove();
        }
        //}
    }
    return -1;
}

} // namespace kernel

void *operator new(size_t size) {
    return kernel::a.allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void operator delete(void *address) noexcept {
    kernel::a.free(address);
}

void operator delete(void *address, size_t) noexcept {
    kernel::a.free(address);
}

void *operator new[](size_t size) {
    return kernel::a.allocate(size);
}

void operator delete[](void *address) noexcept {
    kernel::a.free(address);
}

void operator delete[](void *address, size_t) noexcept {
    kernel::a.free(address);
}

