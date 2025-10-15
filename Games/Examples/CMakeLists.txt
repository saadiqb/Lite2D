# Example Games CMakeLists.txt

# Bouncing Balls Example
add_executable(bouncing_balls BouncingBalls/test_systems.cpp)
target_link_libraries(bouncing_balls PRIVATE Lite2D)
