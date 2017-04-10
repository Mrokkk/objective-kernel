set_property(SOURCE head.S PROPERTY LANGUAGE C)
set_property(SOURCE cpu/exceptions.S PROPERTY LANGUAGE C)
add_library(kernel OBJECT head.S cpp_support.cpp console/console.cpp cpu/stack_frame.cpp cpu/gdt.cpp cpu/exceptions.cpp cpu/exceptions.S cpu/idt.cpp
    memory/allocator.cpp memory/paging.cpp scheduler/process.cpp vfs/vfs.cpp vfs/block_device.cpp vfs/ramfs.cpp printf.cpp)

