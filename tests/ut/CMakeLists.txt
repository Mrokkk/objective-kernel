set(CMAKE_EXE_LINKER_FLAGS "")

if(COVERAGE)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(COVERAGE_FLAGS "-O0 -fprofile-instr-generate -fcoverage-mapping -fno-inline")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
    else()
        set(COVERAGE_FLAGS "-O0 -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
    endif()
endif()

if(SANITIZERS)
    set(SANITIZERS_FLAGS "-fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZERS_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -std=c++1z -Wall -Wextra ${COVERAGE_FLAGS} ${SANITIZERS_FLAGS}")
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

if(COVERAGE)
    add_custom_target(clean-coverage
        COMMAND find ${CMAKE_BINARY_DIR} -name '*.gcda' -exec rm {} "\;"
        DEPENDS ${PROJECT_NAME}-ut
        COMMENT "Cleaning coverage data")
    add_dependencies(ut-run clean-coverage)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_custom_target(tests-cov
            DEPENDS ${PROJECT_NAME}-ut
            COMMAND LLVM_PROFILE_FILE=tests.profdata ./${PROJECT_NAME}-ut
            COMMAND llvm-profdata merge -instr tests.profdata -o merged.profdata
            COMMAND llvm-cov report ./${PROJECT_NAME}-ut -instr-profile=merged.profdata
            COMMENT "Running LLVM coverage generating")
    else()
        add_custom_target(tests-cov
            COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/kernel ${CMAKE_BINARY_DIR}
            DEPENDS ut-run
            COMMENT "Running GCOVR coverage generating")
        add_custom_target(tests-cov-html
            COMMAND gcovr --html --html-details -o ut.coverage.html -r ${CMAKE_SOURCE_DIR}/kernel ${CMAKE_BINARY_DIR}
            DEPENDS ut-run
            COMMENT "Running GCOVR coverage generating (HTML)")
    endif()
endif()

