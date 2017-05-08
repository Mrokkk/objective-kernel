#include <kernel/cpu/stack_frame.hpp>
#include <kernel/console/console.hpp>

namespace cpu {

console::console &operator<<(console::console &c, const stack_frame &s) {
    c << "EAX=" << s.eax << "; ";
    c << "EBX=" << s.ebx << "; ";
    c << "ECX=" << s.ecx << "; ";
    c << "EDX=" << s.edx << "\n";
    c << "ESP=" << s.ss << ':' << s.esp << "\n";
    c << "EIP=" << s.cs << ':' << s.eip << "\n";
    c << "DS=" << s.ds << "; ES=" << s.es << "; FS=" << s.fs << "; GS=" << s.gs << "\n";
    c << "EFLAGS=" << s.eflags << " ";
    c << "IOPL=" << ((s.eflags >> 12) & 0x3) << "\n";
    return c;
}

} // namespace cpu

