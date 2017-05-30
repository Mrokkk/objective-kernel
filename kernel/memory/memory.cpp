#include <allocator.hpp>

#include <kernel/logger/logger.hpp>

#include "memory.hpp"
#include "paging/paging.hpp"

namespace memory {

namespace {

logger log;

} // namespace

using allocator = utils::allocator<paging::page_allocator, 32>;

allocator kernel_allocator(nullptr);

void initialize() {
    log.set_name("memory");
    paging::initialize();
}

} // namespace memory

