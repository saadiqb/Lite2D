# Examples CMakeLists.txt

# Hello World Example
add_executable(hello_example hello.cpp)
target_link_libraries(hello_example PRIVATE Lite2D)

if(WIN32)
    set_target_properties(hello_example PROPERTIES WIN32_EXECUTABLE YES)
endif()
