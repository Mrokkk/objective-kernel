set_property(SOURCE exceptions.S PROPERTY LANGUAGE C)
add_library(cpu OBJECT
    stack_frame.cpp
    gdt.cpp
    exceptions.cpp
    exceptions.S
    idt.cpp
)
