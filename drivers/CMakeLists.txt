add_library(drivers serial.cpp vga.cpp keyboard.cpp)
target_link_libraries(drivers kernel)
