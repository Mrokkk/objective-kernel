#include <kernel/logger/logger.hpp>
#include "registers.hpp"
#include "stack_frame.hpp"

#define exception_errno(x) \
    __STRING_##x,

#define exception_noerrno(x) \
    __STRING_##x,

namespace cpu {

namespace exceptions {

namespace {

const char *exception_names[] = {
    #include "exceptions.hpp"
};

} // namespace

asmlinkage void exception_handler(uint32_t nr, uint32_t error_code, cpu::stack_frame frame) {
    cpu::cli();
    logger("kernel") << logger::error << "Exception: " << exception_names[nr] << " #" << static_cast<int>(error_code) << "\n"
                  << frame << "CR2 = " << registers::cr2_get() << "\n";
    while (1) {
        cpu::halt();
    }
}

} // namespace exceptions

} // namespace cpu

