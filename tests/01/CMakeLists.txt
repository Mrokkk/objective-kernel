include(../modules/coverage.cmake)

set(CMAKE_EXE_LINKER_FLAGS "")

if(SANITIZERS)
    set(SANITIZERS_FLAGS "-fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZERS_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -std=c++1z -Wall -Wextra \
-fno-exceptions -fno-rtti -fno-stack-protector -mno-sse -mno-sse2 \
-mno-sse3 -mno-sse4 -mno-avx ${COVERAGE_FLAGS} ${SANITIZERS_FLAGS}")

if(COVERAGE)
    add_coverage_flags()
endif()

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
    COMPILE_FLAGS "-Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

add_library(ut-vfs
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vfs.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vnode.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/cache.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/file.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/ramfs.cpp
)

set_target_properties(ut-vfs PROPERTIES
    COMPILE_FLAGS "-Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

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

if(COVERAGE)
    add_coverage_targets(${PROJECT_NAME}-ut ut-run ut ${CMAKE_SOURCE_DIR}/kernel)
endif()

