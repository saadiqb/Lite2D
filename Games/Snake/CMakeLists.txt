cmake_minimum_required(VERSION 3.16)
project(SnakeGame)
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
    add_subdirectory(../../vendored/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(../../vendored/SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(../../vendored/SDL_image EXCLUDE_FROM_ALL)
else()
    message(STATUS "Using system-installed SDL3")
endif()

# Snake Game executable
add_executable(snake_game 
    main.cpp
    ../../ECS/EntityManager.cpp
    ../../ECS/SystemManager.cpp
    ../../ECS/Systems/MovementSystem.cpp
    ../../ECS/Systems/RenderSystem.cpp
    Components/SnakeSegment.cpp
    Components/SnakeHead.cpp
    Components/Food.cpp
    Components/Wall.cpp
    Components/GameState.cpp
    Systems/SnakeMovementSystem.cpp
    Systems/CollisionSystem.cpp
    Systems/InputSystem.cpp
    Systems/GameLogicSystem.cpp
    Game/SnakeGame.cpp
)
target_include_directories(snake_game PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS
)
target_compile_features(snake_game PRIVATE cxx_std_17)
target_link_libraries(snake_game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if(WIN32)
    set_target_properties(snake_game PROPERTIES WIN32_EXECUTABLE YES)
    target_compile_definitions(snake_game PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(snake_game PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(snake_game PRIVATE PLATFORM_LINUX)
endif()
