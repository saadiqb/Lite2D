cmake_minimum_required(VERSION 3.16)
project(Lite2D VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

# Platform detection
if(WIN32)
    set(PLATFORM_NAME "Windows")
    set(PLATFORM_DEFINE "PLATFORM_WINDOWS")
elseif(APPLE)
    set(PLATFORM_NAME "macOS")
    set(PLATFORM_DEFINE "PLATFORM_MACOS")
elseif(UNIX)
    set(PLATFORM_NAME "Linux")
    set(PLATFORM_DEFINE "PLATFORM_LINUX")
endif()

message(STATUS "Building Lite2D for ${PLATFORM_NAME}")

# SDL3 Configuration
option(USE_VENDORED_SDL "Use vendored SDL source tree" ON)

if(APPLE OR UNIX)
    find_package(SDL3 QUIET)
    find_package(SDL3_image QUIET)
    find_package(SDL3_ttf QUIET)
    if(SDL3_FOUND AND SDL3_image_FOUND AND SDL3_ttf_FOUND)
        set(USE_VENDORED_SDL OFF)
        message(STATUS "Using system-installed SDL3")
    endif()
endif()

if(USE_VENDORED_SDL)
    message(STATUS "Using vendored SDL3 build")
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Core Lite2D Engine Library
add_library(Lite2D STATIC
    # ECS Core
    src/ECS/EntityManager.cpp
    src/ECS/EntityManager.h
    src/ECS/SystemManager.cpp
    src/ECS/SystemManager.h
    src/ECS/Component.h
    src/ECS/ComponentArray.h
    src/ECS/Entity.h
    src/ECS/IComponentArray.h
    src/ECS/System.h
    
    # ECS Components
    src/ECS/Components/Position.h
    src/ECS/Components/Renderable.h
    src/ECS/Components/Velocity.h
    
    # ECS Systems
    src/ECS/Systems/MovementSystem.cpp
    src/ECS/Systems/MovementSystem.h
    
    # Rendering
    src/Rendering/Renderer.cpp
    src/Rendering/Renderer.h
    src/Rendering/RenderSystem.cpp
    src/Rendering/RenderSystem.h
    src/Rendering/TextRenderer.cpp
    src/Rendering/TextRenderer.h
    src/Rendering/Texture.cpp
    src/Rendering/Texture.h
    
    # Input
    src/Input/InputManager.cpp
    src/Input/InputManager.h
    
    # Utils
    src/Utils/timer.cpp
    src/Utils/timer.h
)

target_include_directories(Lite2D PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Lite2D PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

target_compile_definitions(Lite2D PRIVATE ${PLATFORM_DEFINE})

# Examples
add_subdirectory(examples)

# Games
add_subdirectory(games)

# Tests
enable_testing()
add_subdirectory(tests)