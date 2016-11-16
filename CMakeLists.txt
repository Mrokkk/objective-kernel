project(os C CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-ffreestanding -Wall -Wextra -Werror -g3 -O0 -Wl,-T${CMAKE_SOURCE_DIR}/arch/linker.ld -Wl,-Map=kernel.map -nostartfiles -nostdlib -m32 -Wl,--oformat -Wl,elf32-i386 -Wl,-m -Wl,elf_i386")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++14")

if(CI_BUILD)
    add_definitions("-DCI")
endif()

set_property(SOURCE arch/head.S PROPERTY LANGUAGE C)

include_directories(include)
include_directories(yacppl/include)
include_directories(lib)
include_directories(arch)
include_directories(drivers)

add_executable(${PROJECT_NAME}
    arch/head.S cxx.cpp main.cpp lib/cstring.cpp drivers/vga.cpp
)

add_custom_target(
    runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/${PROJECT_NAME} -serial stdio -no-reboot \\$$@ > ${CMAKE_BINARY_DIR}/runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/${PROJECT_NAME} -display none -serial stdio -no-reboot \\$$@ > ${CMAKE_BINARY_DIR}/runqemu-nographic
    COMMAND chmod +x runqemu*
    DEPENDS ${PROJECT_NAME}
    COMMENT "Creating runqemu executables"
)

