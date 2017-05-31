set_property(SOURCE exceptions.S PROPERTY LANGUAGE C)
add_library(cpu OBJECT
    cpu.cpp
    stack_frame.cpp
    gdt.cpp
    exceptions.cpp
    exceptions.S
    idt.cpp
    irq.cpp
    pit.cpp
    tss.cpp
)
