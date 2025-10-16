# Example Games CMakeLists.txt

# Bouncing Balls Example
add_executable(bouncing_balls BouncingBalls/test_systems.cpp)
target_link_libraries(bouncing_balls PRIVATE Lite2D)

# Colliding Particles Example
add_executable(colliding_particles 
    Particles/main.cpp
    Particles/Game/ParticleGame.cpp
    Particles/Systems/CollisionSystem.cpp
    Particles/Systems/ParticleSystem.cpp
)
target_link_libraries(colliding_particles PRIVATE Lite2D)
