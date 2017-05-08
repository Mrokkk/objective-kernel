#include <allocator.h>
#include <kernel/console/logger.hpp>

#include "paging/paging.hpp"

namespace memory {

using allocator = utils::allocator<paging::page_allocator, 32>;

allocator kernel_allocator(nullptr);

void initialize() {
    paging::initialize();
}

} // namespace memory

