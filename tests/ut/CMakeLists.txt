set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -std=c++1z -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "")
include_directories(${CMAKE_SOURCE_DIR}/yacppl/include)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(../yatf/include)

add_executable(${PROJECT_NAME}-ut
    main.cpp
    console.cpp
    vfs.cpp
)

add_library(ut
    ${CMAKE_SOURCE_DIR}/kernel/printf.cpp
    ${CMAKE_SOURCE_DIR}/kernel/console/console.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vfs.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/block_device.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vnode.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/cache.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/file.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/ramfs.cpp
)

set_target_properties(ut PROPERTIES
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

target_link_libraries(${PROJECT_NAME}-ut ut)

add_custom_target(
    ut-run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-ut
    DEPENDS ${PROJECT_NAME}-ut
    COMMENT "Running UTs"
)

