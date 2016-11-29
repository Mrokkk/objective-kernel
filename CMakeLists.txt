project(os C CXX)
cmake_minimum_required(VERSION 3.0)

option(CROSS_COMPILER_PREFIX "Use GNU cross-compiler" OFF)
message(STATUS "Use GNU cross-compiler: ${CROSS_COMPILER_PREFIX}")

option(OPTIMIZE "Use compile optimizer" OFF)
message(STATUS "Use compile optimizer: ${OPTIMIZE}")

option(MULTIBOOT2 "Use Multiboot2" OFF)
message(STATUS "Use Multiboot2: ${MULTIBOOT2}")

if(CROSS_COMPILER_PREFIX)
    set(CMAKE_C_COMPILER_WORKS 1)
    set(CMAKE_CXX_COMPILER_WORKS 1)
    set(CMAKE_C_COMPILER ${CROSS_COMPILER_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${CROSS_COMPILER_PREFIX}-g++)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
endif()

if(OPTIMIZE)
    set(OPTIMIZE_LEVEL 2)
else()
    set(OPTIMIZE_LEVEL 0)
endif()

set(CMAKE_C_FLAGS "-include ${CMAKE_SOURCE_DIR}/kernel/definitions.h -m32 -fdiagnostics-color=always")

set(CMAKE_CXX_FLAGS "-m32 -fdiagnostics-color=always -std=c++14 -ffreestanding -nostdlib -Wall -Wextra -Werror \
-fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector -mno-sse -mno-sse2 \
-mno-sse3 -mno-sse4 -mno-avx -O${OPTIMIZE_LEVEL} -include ${CMAKE_SOURCE_DIR}/kernel/definitions.h -include ${CMAKE_SOURCE_DIR}/kernel/cpu/common.h")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T${CMAKE_SOURCE_DIR}/linker.ld -Wl,-Map=kernel.map -Wl,--oformat -Wl,elf32-i386 -Wl,-m -Wl,elf_i386 -nostartfiles -nostdlib")

if(MULTIBOOT2)
    add_definitions("-DMULTIBOOT2")
endif()

include_directories(yacppl/include)
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(basic)
add_subdirectory(drivers)
add_subdirectory(kernel)
add_subdirectory(lib)

add_library(${PROJECT_NAME}-lib
    $<TARGET_OBJECTS:basic>
    $<TARGET_OBJECTS:drivers>
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:lib>
)

add_executable(${PROJECT_NAME}
    kernel/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

configure_file(${CMAKE_SOURCE_DIR}/CMake/runqemu.in ${CMAKE_BINARY_DIR}/runqemu @ONLY)

if(MULTIBOOT2)
    set(MULTIBOOT_FLAG "--multiboot2")
endif()

add_custom_target(
    iso
    COMMAND ${CMAKE_SOURCE_DIR}/CMake/isomaker.sh -o ${PROJECT_NAME} ${MULTIBOOT_FLAG}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Building bootable ISO"
)

add_custom_target(
    runqemu
    COMMAND chmod +x ${CMAKE_BINARY_DIR}/runqemu
    COMMAND ${CMAKE_BINARY_DIR}/runqemu
    DEPENDS iso
    COMMENT "Booting kernel"
)

add_subdirectory(tests)

