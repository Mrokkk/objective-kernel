include_directories(../yatf/include)
add_executable(${PROJECT_NAME}-ut main.cpp)

add_custom_target(
    ut-run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-ut
    DEPENDS ${PROJECT_NAME}-ut
    COMMENT "Running UTs"
)

