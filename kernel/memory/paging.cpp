#include <algorithm.h>
#include <kernel/memory/paging.hpp>

extern "C" char page_dir[];
char *_page_dir = page_dir + KERNEL_PAGE_OFFSET;

namespace memory {

void initialize() {
    utils::fill(_page_dir, 4, 0);
}

} // namespacce memory

