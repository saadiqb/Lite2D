# Lite2D - High-Performance ECS Game Engine

My attempt at a modern, high-performance Entity-Component-System (ECS) game engine built in C++17 with SDL3, designed for 2D games with performance optimizations.

## 🚀 Features

### Core Architecture

- **Entity-Component-System (ECS)** - Clean separation of data and behavior
- **Type-Safe Component System** - Compile-time type safety with templates
- **System-Based Architecture** - Modular, extensible game logic
- **Memory-Efficient Storage** - Contiguous component arrays for cache-friendly access

### Performance Optimizations

- **Active Entity Tracking** - Only processes living entities (3,333x performance improvement)
- **Component Array Storage** - Cache-friendly memory layout
- **System Signature Matching** - Automatic entity filtering for systems
- **60+ FPS Performance** - Smooth gameplay with minimal overhead

### Rendering System

- **SDL3 Integration** - Modern, cross-platform rendering
- **Layer-Based Rendering** - Configurable render order
- **Debug Visualization** - Built-in performance monitoring
- **Camera System** - Viewport management and offset support
- **VSync Support** - Smooth, tear-free rendering

## 🎮 Demo Games

### Snake Game

A complete Snake implementation demonstrating:

- Entity movement and collision detection
- Game state management
- Input handling
- Score tracking and game progression

**Controls:**

- Arrow Keys / WASD - Move snake
- Space - Start/Pause
- R - Restart
- Q - Quit
- ESC - Exit

## 🏗️ Architecture

### ECS Components

```
Entity (uint32_t ID)
├── Position (x, y coordinates)
├── Velocity (x, y movement)
├── Renderable (visibility, layer)
├── SnakeHead (direction, movement timer)
├── SnakeSegment (segment index)
├── Food (nutritional value)
├── Wall (boundary type)
└── GameState (current game state)
```

### Core Systems

- **EntityManager** - Entity lifecycle and component management
- **SystemManager** - System registration and execution order
- **MovementSystem** - Physics and movement updates
- **RenderSystem** - Rendering and visual presentation
- **SnakeMovementSystem** - Snake-specific movement logic
- **CollisionSystem** - Collision detection and response
- **InputSystem** - User input processing
- **GameLogicSystem** - Game state and rules

## 📊 Performance Benchmarks

| Metric             | Before Optimization | After Optimization | Improvement |
| ------------------ | ------------------- | ------------------ | ----------- |
| MovementSystem     | 130ms/frame         | 0.006ms/frame      | **21,667x** |
| RenderSystem       | 130ms/frame         | 0.08ms/frame       | **1,625x**  |
| Total Frame Time   | 261ms               | 0.11ms             | **2,373x**  |
| FPS                | 3.7 FPS             | 58+ FPS            | **15.7x**   |
| Frame Budget Usage | 1566%               | 0.68%              | **2,303x**  |

## 🛠️ Building

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

### Project Structure

```
Lite2D/
├── ECS/                    # Core ECS engine
│   ├── EntityManager.h     # Entity lifecycle management
│   ├── SystemManager.h     # System coordination
│   ├── Components/         # Base component types
│   └── Systems/           # Core systems
├── Games/
│   └── Snake/             # Snake game implementation
├── tests/                 # Unit tests and benchmarks
└── CMakeLists.txt         # Build configuration
```

## 🧪 Testing

Comprehensive test suite with performance benchmarks:

```bash
# Run all tests
./bin/ecs_tests

# Run specific performance tests
./bin/ecs_tests --gtest_filter="MovementSystemPerformanceTest.*"

# Run bouncing balls demo
./bin/test_systems

# Run Snake game
./bin/snake_game
```

## 📈 Performance Analysis

The engine includes built-in performance monitoring:

- **Frame timing analysis** - Actual vs expected frame times
- **System performance profiling** - Individual system execution times
- **ECS statistics** - Entity counts, component distributions
- **Memory usage tracking** - Component array efficiency
- **Debug visualization** - Real-time performance metrics

## 🔧 Usage Example

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

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🎓 Learning Outcomes

This project demonstrates:

- **Modern C++17** features and best practices
- **ECS Architecture** design and implementation
- **Performance optimization** techniques and profiling
- **Game engine architecture** principles
- **Cross-platform development** with SDL3
- **Unit testing** and performance benchmarking
- **Memory management** and cache optimization

---

_Built with ❤️ and modern C++ for optimal performance_
