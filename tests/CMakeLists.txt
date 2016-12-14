add_executable(${PROJECT_NAME}-tests
    main.cpp
)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME}-lib)

configure_file(${CMAKE_SOURCE_DIR}/CMake/runtests.in ${CMAKE_BINARY_DIR}/runtests @ONLY)

add_custom_target(
    tests-iso
    COMMAND ${CMAKE_SOURCE_DIR}/CMake/isomaker.sh -o ${PROJECT_NAME}-tests ${MULTIBOOT_FLAG} --grub-use-serial
    DEPENDS ${PROJECT_NAME}-tests
    COMMENT "Building bootable ISO for tests"
)

add_custom_target(
    runtests
    COMMAND chmod +x ${CMAKE_BINARY_DIR}/runtests
    COMMAND ${CMAKE_BINARY_DIR}/runtests
    DEPENDS tests-iso
    COMMENT "Running tests"
)

