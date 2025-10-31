# Lite2D - High-Performance ECS Game Engine

My attempt at a modern, high-performance Entity-Component-System (ECS) game engine built in C++17 with SDL3, designed for 2D games with performance optimizations.

![Visualization](https://raw.githubusercontent.com/saadiqb/saadiqb.github.io/refs/heads/master/src/assets/images/ParticleCollision.gif)


### Core Architecture

- **Entity-Component-System (ECS)** - Clean separation of data and behavior
- **Type-Safe Component System** - Compile-time type safety with templates
- **System-Based Architecture** - Modular, extensible game logic
- **Memory-Efficient Storage** - Contiguous component arrays for cache-friendly access

### Rendering System

- **SDL3 Integration** - Modern, cross-platform rendering
- **Layer-Based Rendering** - Configurable render order
- **Debug Visualization** - Built-in performance monitoring
- **Camera System** - Viewport management and offset support
- **VSync Support** - Smooth, tear-free rendering

## Demo

Particles Collision
Snake Game

### Core Systems

- **EntityManager** - Entity lifecycle and component management
- **SystemManager** - System registration and execution order
- **MovementSystem** - Physics and movement updates
- **RenderSystem** - Rendering and visual presentation
- **SnakeMovementSystem** - Snake-specific movement logic
- **CollisionSystem** - Collision detection and response
- **InputSystem** - User input processing
- **GameLogicSystem** - Game state and rules

## Performance Benchmarks

stress testing & benchmarking (in-progress) ...

## Building

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16+
- SDL3 development libraries

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/saadiqb/Lite2D.git
cd Lite2D

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc)

# Run tests
./bin/ecs_tests

# Run demo applications
./bin/test_systems    # Bouncing balls animation
./bin/snake_game      # Snake game demo
```

## Testing

```bash
# Run all tests
./bin/ecs_tests

# Run specific performance tests
./bin/ecs_tests --gtest_filter="MovementSystemPerformanceTest.*"


# Run Snake game
./bin/snake_game
```

## Performance Analysis

The engine includes built-in performance monitoring:

- **Frame timing analysis** - Actual vs expected frame times
- **System performance profiling** - Individual system execution times
- **ECS statistics** - Entity counts, component distributions
- **Memory usage tracking** - Component array efficiency
- **Debug visualization** - Real-time performance metrics

## Usage Example

```cpp
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/RenderSystem.h"

// Create ECS managers
EntityManager entityManager;
SystemManager systemManager;

// Register systems
auto movementSystem = systemManager.RegisterSystem<MovementSystem>();
auto renderSystem = systemManager.RegisterSystem<RenderSystem>(renderer);

// Set system signatures
systemManager.SetSystemSignature<MovementSystem>(
    entityManager.GetComponentSignature<Position, Velocity>()
);

// Create entities
Entity player = entityManager.CreateEntity();
entityManager.AddComponent(player, Position(100, 100));
entityManager.AddComponent(player, Velocity(50, 30));
entityManager.AddComponent(player, Renderable(true, 1));

// Game loop
while (running) {
    systemManager.UpdateSystems(entityManager, deltaTime);
}
```
