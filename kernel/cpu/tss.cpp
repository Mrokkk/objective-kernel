#include <algorithm.hpp>
#include "tss.hpp"

namespace cpu {

tss::tss() {
    utils::fill(io_bitmap, io_bitmap + 128, 0xff);
}

tss::tss(void *kernel_stack) : esp0(reinterpret_cast<uint32_t>(kernel_stack)) {
    utils::fill(io_bitmap, io_bitmap + 128, 0xff);
}

void tss::load() const {
    asm volatile("ltr %%ax" :: "a" (segment::task));
}

} // namespace cpu

