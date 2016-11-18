project(os C CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-ffreestanding -Wall -Wextra -Werror -fno-exceptions -g3 -O0 -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++14 -fdiagnostics-color=always")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T${CMAKE_SOURCE_DIR}/arch/linker.ld -Wl,-Map=kernel.map -Wl,--oformat -Wl,elf32-i386 -Wl,-m -Wl,elf_i386 -nostartfiles -nostdlib")

if(CI_BUILD)
    add_definitions("-DCI")
endif()

set_property(SOURCE arch/head.S PROPERTY LANGUAGE C)

include_directories(yacppl/include)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME}
    arch/head.S cxx.cpp kernel/main.cpp kernel/allocator.cpp lib/cstring.cpp lib/printf.cpp drivers/vga.cpp
)

add_custom_target(
    runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/${PROJECT_NAME} -serial stdio -no-reboot -cpu core2duo \\$$@ > ${CMAKE_BINARY_DIR}/runqemu
    COMMAND echo exec qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/${PROJECT_NAME} -display none -serial stdio -no-reboot -cpu core2duo \\$$@ > ${CMAKE_BINARY_DIR}/runqemu-nographic
    COMMAND chmod +x runqemu*
    DEPENDS ${PROJECT_NAME}
    COMMENT "Creating runqemu executables"
)

add_custom_target(
    iso
    COMMAND ${CMAKE_SOURCE_DIR}/CMake/isomaker.sh
    DEPENDS ${PROJECT_NAME}
    COMMENT "Building bootable ISO"
)

add_custom_target(
    runqemu-iso
    COMMAND echo exec qemu-system-i386 -cdrom ${CMAKE_BINARY_DIR}/os.iso -serial stdio -no-reboot -cpu core2duo \\$$@ > ${CMAKE_BINARY_DIR}/runqemu-iso
    COMMAND echo exec qemu-system-i386 -cdrom ${CMAKE_BINARY_DIR}/os.iso -display none -serial stdio -no-reboot -cpu core2duo \\$$@ > ${CMAKE_BINARY_DIR}/runqemu-iso-nographic
    COMMAND chmod +x runqemu*
    DEPENDS iso
    COMMENT "Creating runqemu executables for ISO"
)

