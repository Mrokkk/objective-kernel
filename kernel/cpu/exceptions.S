#include <kernel/cpu/exceptions.h>
#include <kernel/cpu/gdt.h>

define_exception_noerrno(divide_error)
define_exception_noerrno(debug)
define_exception_noerrno(nmi)
define_exception_noerrno(breakpoint)
define_exception_noerrno(overflow)
define_exception_noerrno(bound_range)
define_exception_noerrno(invalid_opcode)
define_exception_noerrno(device_na)
define_exception_errno(double_fault)
define_exception_noerrno(coprocessor)
define_exception_errno(invalid_tss)
define_exception_errno(segment_np)
define_exception_errno(stack_segment)
define_exception_errno(general_protection)
define_exception_errno(page_fault)
