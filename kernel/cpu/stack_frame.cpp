#include <kernel/cpu/stack_frame.h>
#include <kernel/console.h>

namespace cpu {

void stack_frame::print() {
    console::print("EAX=0x%08x; ", eax);
    console::print("EBX=0x%08x; ", ebx);
    console::print("ECX=0x%08x; ", ecx);
    console::print("EDX=0x%08x\n", edx);
    console::print("ESP=0x%04x:0x%08x; ", ss, esp);
    console::print("EIP=0x%04x:0x%08x\n", cs, eip);
    console::print("DS=0x%04x; ES=0x%04x; FS=0x%04x; GS=0x%04x\n", ds, es, fs, gs);
    console::print("EFLAGS=0x%08x : ", eflags);
    console::print("IOPL=%d\n", (eflags >> 12) & 0x3);
}

} // namespace cpu

