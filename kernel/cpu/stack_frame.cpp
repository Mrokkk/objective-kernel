#include <kernel/cpu/stack_frame.hpp>
#include <kernel/console/console.hpp>

namespace cpu {

console::console &operator<<(console::console &c, const stack_frame &s) {
    return c << "EAX=" << s.eax << "; "
        << "EBX=" << s.ebx << "; "
        << "ECX=" << s.ecx << "; "
        << "EDX=" << s.edx << "\n"
        << "ESP=" << s.ss << ':' << s.esp << "\n"
        << "EIP=" << s.cs << ':' << s.eip << "\n"
        << "DS=" << s.ds << "; ES=" << s.es << "; FS=" << s.fs << "; GS=" << s.gs << "\n"
        << "EFLAGS=" << s.eflags << " "
        << "IOPL=" << ((s.eflags >> 12) & 0x3) << "\n";
}

} // namespace cpu

