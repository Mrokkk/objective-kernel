add_subdirectory(boot)
add_subdirectory(console)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(logger)
add_subdirectory(scheduler)
add_subdirectory(time)
add_subdirectory(vfs)

set_property(SOURCE head.S PROPERTY LANGUAGE C)
add_library(kernel head.S cxx.cpp kernel.cpp
    $<TARGET_OBJECTS:boot>
    $<TARGET_OBJECTS:console>
    $<TARGET_OBJECTS:cpu>
    $<TARGET_OBJECTS:memory>
    $<TARGET_OBJECTS:logger>
    $<TARGET_OBJECTS:scheduler>
    $<TARGET_OBJECTS:time>
    $<TARGET_OBJECTS:vfs>
)

