cmake_minimum_required(VERSION 3.16)

# Find Google Test
find_package(GTest REQUIRED)

# Create test executable
add_executable(ecs_tests
    # Test files
    test_entity_manager.cpp
    test_component_array.cpp
    test_system_manager.cpp
    test_movement_system_performance.cpp
    test_main.cpp
    
    # Source files to test
    ${CMAKE_SOURCE_DIR}/ECS/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/ECS/SystemManager.cpp
    ${CMAKE_SOURCE_DIR}/ECS/Systems/MovementSystem.cpp
    ${CMAKE_SOURCE_DIR}/ECS/Systems/RenderSystem.cpp
)

# Include directories
target_include_directories(ecs_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

# Link libraries - use modern CMake targets
target_link_libraries(ecs_tests 
    GTest::GTest
    GTest::Main
    SDL3::SDL3
)

# Compile features
target_compile_features(ecs_tests PRIVATE cxx_std_17)

# Add test to CTest
add_test(NAME ECS_Tests COMMAND ecs_tests)
