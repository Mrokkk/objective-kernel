set_property(SOURCE head.S PROPERTY LANGUAGE C)
set_property(SOURCE cpu/exceptions.S PROPERTY LANGUAGE C)
add_library(kernel OBJECT head.S console.cpp cpu/gdt.cpp cpu/exceptions.cpp cpu/exceptions.S cpu/idt.cpp memory/allocator.cpp memory/paging.cpp)

