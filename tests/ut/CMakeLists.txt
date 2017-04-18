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

add_library(ut-common
    ${CMAKE_SOURCE_DIR}/kernel/printf.cpp
    ${CMAKE_SOURCE_DIR}/kernel/console/console.cpp

)

set_target_properties(ut-common PROPERTIES
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

add_library(ut-vfs
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vfs.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/block_device.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vnode.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/cache.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/file.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/ramfs.cpp
)

set_target_properties(ut-vfs PROPERTIES
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

target_link_libraries(${PROJECT_NAME}-ut ut-common ut-vfs)

add_custom_target(
    ut-run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-ut
    DEPENDS ${PROJECT_NAME}-ut
    COMMENT "Running UTs"
)

add_custom_target(
    ut-valgrind
    DEPENDS ${PROJECT_NAME}-ut
    COMMAND valgrind ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-ut
    COMMENT "Running UTs with valgrind"
)

