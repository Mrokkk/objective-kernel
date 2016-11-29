set_property(SOURCE head.S PROPERTY LANGUAGE C)
add_library(kernel OBJECT head.S cpu/gdt.cpp memory/allocator.cpp memory/paging.cpp)

