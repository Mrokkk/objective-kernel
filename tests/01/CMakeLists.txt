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

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=tests.map")

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
    scheduler.cpp
)

file(GLOB STUBS stubs/*.cpp)

add_library(ut-common SHARED
    ${STUBS}
    ${CMAKE_SOURCE_DIR}/kernel/time/time.cpp
    ${CMAKE_SOURCE_DIR}/kernel/logger/logger.cpp
    ${CMAKE_SOURCE_DIR}/kernel/console/printf.cpp
    ${CMAKE_SOURCE_DIR}/kernel/console/console.cpp
    ${CMAKE_SOURCE_DIR}/kernel/interfaces/component.cpp
    ${CMAKE_SOURCE_DIR}/kernel/interfaces/scheduler.cpp
)

set_target_properties(ut-common PROPERTIES
    COMPILE_FLAGS "-Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

add_library(ut-vfs SHARED
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vfs.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/vnode.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/cache.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/file.cpp
    ${CMAKE_SOURCE_DIR}/kernel/vfs/ramfs.cpp
)

add_library(ut-scheduler SHARED
    ${CMAKE_SOURCE_DIR}/kernel/scheduler/scheduler.cpp
)

set_target_properties(ut-vfs PROPERTIES
    COMPILE_FLAGS "-Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

target_link_libraries(${PROJECT_NAME}-ut ut-common ut-vfs ut-scheduler)

set_target_properties(ut-scheduler PROPERTIES
    COMPILE_FLAGS "-Werror -include ${CMAKE_SOURCE_DIR}/kernel/definitions.hpp -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.hpp")

add_run_target(${PROJECT_NAME}-ut ut-run)
if (NOT SANITIZERS)
    add_valgrind_target(${PROJECT_NAME}-ut ut-valgrind)
endif()
if(COVERAGE)
    add_coverage_targets(${PROJECT_NAME}-ut ut-run ut ${CMAKE_SOURCE_DIR}/kernel
        # Excluded files
        ${CMAKE_SOURCE_DIR}/kernel/component.cpp
        ${CMAKE_SOURCE_DIR}/kernel/time/time.cpp
        ${CMAKE_SOURCE_DIR}/kernel/logger/logger.cpp
        ${CMAKE_SOURCE_DIR}/kernel/logger/logger.hpp
        ${CMAKE_SOURCE_DIR}/kernel/console/printf.cpp
        ${CMAKE_SOURCE_DIR}/kernel/console/console.cpp
        ${CMAKE_SOURCE_DIR}/kernel/console/console.hpp)
endif()

