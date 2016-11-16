cmake_minimum_required(VERSION 3.5)
project(os)

enable_language(C)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_FLAGS "-ffreestanding -Wall -Wextra -Werror -g3 -O0 -Wl,-T${CMAKE_SOURCE_DIR}/arch/linker.ld -Wl,-Map=kernel.map -nostartfiles -m32 -Wl,--oformat -Wl,elf32-i386 -Wl,-m -Wl,elf_i386")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++14")

set_property(SOURCE arch/head.S PROPERTY LANGUAGE C)

include_directories(include)
include_directories(yacppl/include)

add_executable(os arch/head.S cxx.cpp main.cpp)

add_custom_target(
    runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/os -serial stdio -no-reboot \\$$@ > ${CMAKE_BINARY_DIR}/runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/os -display none -serial stdio -no-reboot \\$$@ > ${CMAKE_BINARY_DIR}/runqemu-nographic
    COMMAND chmod +x runqemu*
    DEPENDS os
    COMMENT "Creating runqemu executables"
)

