# Tests CMakeLists.txt

# Find Google Test
find_package(GTest REQUIRED)

# Unit Tests
add_executable(ecs_unit_tests
    unit/test_entity_manager.cpp
    unit/test_component_array.cpp
    unit/test_system_manager.cpp
    unit/test_main.cpp
)

target_link_libraries(ecs_unit_tests 
    PRIVATE 
    Lite2D
    GTest::GTest
    GTest::Main
)

# Performance Tests
add_executable(ecs_performance_tests
    unit/test_movement_system_performance.cpp
    unit/test_main.cpp
)

target_link_libraries(ecs_performance_tests 
    PRIVATE 
    Lite2D
    GTest::GTest
    GTest::Main
)

# Integration Tests
add_executable(ecs_integration_tests
    unit/test_ecs.cpp
)

target_link_libraries(ecs_integration_tests 
    PRIVATE 
    Lite2D
    GTest::GTest
    GTest::Main
)

# Add tests to CTest
add_test(NAME ECS_Unit_Tests COMMAND ecs_unit_tests)
add_test(NAME ECS_Performance_Tests COMMAND ecs_performance_tests)
add_test(NAME ECS_Integration_Tests COMMAND ecs_integration_tests)