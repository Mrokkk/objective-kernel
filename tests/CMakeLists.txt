add_subdirectory(ut)
add_executable(${PROJECT_NAME}-tests
    main.cpp
    vfs.cpp
    ramfs.cpp
)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME}-lib)

configure_file(${CMAKE_SOURCE_DIR}/scripts/runtests.in ${CMAKE_BINARY_DIR}/runtests @ONLY)

add_custom_target(
    tests-iso
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/create_test_images.sh ${PROJECT_NAME}-tests ${MULTIBOOT_FLAG}
    DEPENDS ${PROJECT_NAME}-tests
    COMMENT "Building bootable ISO images for tests"
)

add_custom_target(
    runtests
    COMMAND chmod +x ${CMAKE_BINARY_DIR}/runtests
    COMMAND ${CMAKE_BINARY_DIR}/runtests
    DEPENDS tests-iso
    COMMENT "Running tests"
)

