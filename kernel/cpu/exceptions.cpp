#include <kernel/cpu/exceptions.h>
#include <kernel/cpu/stack_frame.hpp>
#include <kernel/console/console.hpp>
#include "registers.hpp"

const char *exception_names[] = {
    exception_name(divide_error),
    exception_name(debug),
    exception_name(nmi),
    exception_name(breakpoint),
    exception_name(overflow),
    exception_name(bound_range),
    exception_name(invalid_opcode),
    exception_name(device_na),
    exception_name(double_fault),
    exception_name(coprocessor),
    exception_name(invalid_tss),
    exception_name(segment_np),
    exception_name(stack_segment),
    exception_name(general_protection),
    exception_name(page_fault)
};

asmlinkage void exception_handler(uint32_t nr, uint32_t error_code, cpu::stack_frame frame) {
    console::cout << "Exception: " << exception_names[nr] << " #" << static_cast<int>(error_code) << "\n"
                  << frame << "CR2 = " << registers::cr2_get() << "\n";
    while (1);
}

