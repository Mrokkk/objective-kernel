#include <kernel/cpu/stack_frame.hpp>
#include <kernel/console/console.hpp>

namespace cpu {

void stack_frame::print() {
    console::cout << "EAX=" << eax << "; ";
    console::cout << "EBX=" << ebx << "; ";
    console::cout << "ECX=" << ecx << "; ";
    console::cout << "EDX=" << edx << "\n";
    console::cout << "ESP=" << ss << ':' << esp << "\n";
    console::cout << "EIP=" << cs << ':' << eip << "\n";
    console::cout << "DS=" << ds << "; ES=" << es << "; FS=" << fs << "; GS=" << gs << "\n";
    console::cout << "EFLAGS=" << eflags << " ";
    console::cout << "IOPL=" << ((eflags >> 12) & 0x3) << "\n";
}

} // namespace cpu

