#include <algorithm.hpp>
#include "tss.hpp"
#include "gdt.hpp"

namespace cpu {

tss::tss() {
    utils::fill(io_bitmap, io_bitmap + 128, 0xff);
}

tss::tss(void* kernel_stack)
        : esp0(reinterpret_cast<uint32_t>(kernel_stack)) {
    utils::fill(io_bitmap, io_bitmap + 128, 0xff);
}

void tss::load() const {
    asm volatile("ltr %%ax" :: "a" (segment::task));
}

asmlinkage void FASTCALL(__context_switch(tss*, tss* next)) {
    gdt::set_tss(*next);
}

void context_switch(tss& prev, tss& next) {
    asm volatile(R"(
        push %%gs
        pushl %%ebx
        pushl %%ecx
        pushl %%esi
        pushl %%edi
        pushl %%ebp
        movl %%esp, %0
        movl %2, %%esp
        movl $1f, %1
        pushl %3
        jmp __context_switch
        1:
        popl %%ebp
        popl %%edi
        popl %%esi
        popl %%ecx
        popl %%ebx
        pop %%gs)"
        : "=m" (prev.esp),
          "=m" (prev.eip)
        : "m" (next.esp),
          "m" (next.eip),
          "a" (&prev), "d" (&next));
}

} // namespace cpu

