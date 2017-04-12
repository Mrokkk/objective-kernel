add_subdirectory(console)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(scheduler)
add_subdirectory(vfs)

set_property(SOURCE head.S PROPERTY LANGUAGE C)
add_library(kernel head.S cpp_support.cpp printf.cpp boot.cpp
    $<TARGET_OBJECTS:console>
    $<TARGET_OBJECTS:cpu>
    $<TARGET_OBJECTS:memory>
    $<TARGET_OBJECTS:scheduler>
    $<TARGET_OBJECTS:vfs>
)

