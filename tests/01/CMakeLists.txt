include(../cmake-utils/utils.cmake)
include(../cmake-utils/sanitizers.cmake)
include(../cmake-utils/coverage.cmake)

option(SANITIZERS "Use sanitizers" OFF)
message(STATUS "[SANITIZERS] Use sanitizers: ${SANITIZERS}")

option(COVERAGE "Generate coverage report" OFF)
message(STATUS "[COVERAGE] Generate coverage report: ${COVERAGE}")

set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -std=c++1z -Wall -Wextra \
-fno-exceptions -fno-rtti -fno-stack-protector -mno-sse -mno-sse2 \
-mno-sse3 -mno-sse4 -mno-avx -g3")

if(SANITIZERS)
    add_sanitizers_flags()
endif()

if(COVERAGE)
    add_coverage_flags()
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/yacppl/include
    ${CMAKE_SOURCE_DIR}
    ../yatf/include
)

add_executable(${PROJECT_NAME}-ut
    main.cpp
    console.cpp
    vfs.cpp
    ramfs.cpp
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

add_run_target(${PROJECT_NAME}-ut ut-run)
if (NOT SANITIZERS)
    add_valgrind_target(${PROJECT_NAME}-ut ut-valgrind)
endif()
if(COVERAGE)
    add_coverage_targets(${PROJECT_NAME}-ut ut-run ut ${CMAKE_SOURCE_DIR}/kernel
        # Excluded files
        ${CMAKE_SOURCE_DIR}/kernel/printf.cpp
        ${CMAKE_SOURCE_DIR}/kernel/console/console.cpp
        ${CMAKE_SOURCE_DIR}/kernel/console/console.hpp)
endif()

