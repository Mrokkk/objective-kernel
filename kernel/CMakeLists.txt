add_subdirectory(boot)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(logger)
add_subdirectory(scheduler)
add_subdirectory(time)
add_subdirectory(vfs)
add_subdirectory(device)
add_subdirectory(interrupt)
add_subdirectory(interfaces)
add_subdirectory(glue)

set_property(SOURCE head.S PROPERTY LANGUAGE C)
add_library(kernel head.S kernel.cpp cmdline_parser.cpp
    $<TARGET_OBJECTS:boot>
    $<TARGET_OBJECTS:cpu>
    $<TARGET_OBJECTS:memory>
    $<TARGET_OBJECTS:logger>
    $<TARGET_OBJECTS:scheduler>
    $<TARGET_OBJECTS:time>
    $<TARGET_OBJECTS:vfs>
    $<TARGET_OBJECTS:device>
    $<TARGET_OBJECTS:interrupt>
    $<TARGET_OBJECTS:interfaces>
    $<TARGET_OBJECTS:glue>
)

