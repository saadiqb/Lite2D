cmake_minimum_required(VERSION 3.16)
project(hello)
set(CMAKE_CXX_STANDARD 17)

# Output dirs
if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

option(USE_VENDORED_SDL "Use vendored SDL source tree" ON)

if(APPLE OR UNIX)
    find_package(SDL3 QUIET)
    find_package(SDL3_image QUIET)
    find_package(SDL3_ttf QUIET)
    if(SDL3_FOUND AND SDL3_image_FOUND AND SDL3_ttf_FOUND)
        set(USE_VENDORED_SDL OFF)
    endif()
endif()

if(USE_VENDORED_SDL)
    message(STATUS "Using vendored SDL3 build")
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
else()
    message(STATUS "Using system-installed SDL3")
endif()

# Main application
add_executable(hello ${CMAKE_CURRENT_SOURCE_DIR}/hello.cpp)
target_include_directories(hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(hello PRIVATE cxx_std_17)

# ECS test executable
add_executable(test_ecs 
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ecs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/EntityManager.cpp
)
target_include_directories(test_ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(test_ecs PRIVATE cxx_std_17)

# ECS Systems test executable
add_executable(test_systems 
    ${CMAKE_CURRENT_SOURCE_DIR}/test_systems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/SystemManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/MovementSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/RenderSystem.cpp
)
target_include_directories(test_systems PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(test_systems PRIVATE cxx_std_17)
target_link_libraries(test_systems PRIVATE SDL3::SDL3)

target_link_libraries(hello PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if(WIN32)
    set_target_properties(hello PROPERTIES WIN32_EXECUTABLE YES)
    target_compile_definitions(hello PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(hello PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(hello PRIVATE PLATFORM_LINUX)
endif()
