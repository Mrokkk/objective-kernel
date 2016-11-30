#include <kernel/cpu/stack_frame.h>
#include <kernel/console.h>

namespace cpu {

void stack_frame::print() {
    console::print("EAX=", eax, "; ");
    console::print("EBX=", ebx, "; ");
    console::print("ECX=", ecx, "; ");
    console::print("EDX=", edx, "\n");
    console::print("ESP=", ss, ':', esp, "\n");
    console::print("EIP=", cs, ':', eip, "\n");
    console::print("DS=", ds, "; ES=", es, "; FS=", fs, "; GS=", gs, "\n");
    console::print("EFLAGS=", eflags, " ");
    console::print("IOPL=", (eflags >> 12) & 0x3, "\n");
}

} // namespace cpu

