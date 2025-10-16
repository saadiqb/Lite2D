# Colliding Particles Animation

A stress-testing example for the Lite2D engine that demonstrates a complex particle system with collision detection and physics simulation.

## Features

- **Real-time Collision Detection**: Particle-to-particle and particle-to-boundary collisions
- **Physics Simulation**: Mass, velocity, elasticity, and friction
- **Particle Lifecycle Management**: Spawning, lifetime tracking, and cleanup
- **Interactive Controls**: Mouse and keyboard input for spawning particles
- **Multiple Demo Modes**: Different stress-testing scenarios
- **Performance Monitoring**: FPS counter and collision statistics
- **Configurable Parameters**: Adjustable particle properties and system behavior

## Architecture

### Components

- **Particle**: Physical properties (radius, mass, color, lifetime)
- **Position**: 2D coordinates
- **Velocity**: 2D movement vector
- **Renderable**: Visual rendering properties

### Systems

- **ParticleSystem**: Manages particle spawning, lifecycle, and cleanup
- **CollisionSystem**: Handles collision detection and resolution
- **MovementSystem**: Updates positions based on velocity
- **RenderSystem**: Renders particles to screen

### Game Class

- **ParticleGame**: Main orchestrator that manages SDL, ECS, and game loop

## Controls

- **Mouse Left Click**: Spawn particles at cursor position
- **SPACE**: Spawn particle burst at screen center
- **R**: Reset/Clear all particles
- **1**: Basic Demo Mode (100 particles)
- **2**: Stress Test Mode (1000 particles)
- **3**: Burst Demo Mode (periodic bursts)
- **4**: Rainbow Demo Mode (colorful particles)
- **S**: Print performance statistics
- **ESC**: Exit application

## Demo Modes

### Basic Demo

- Moderate particle count (100)
- Steady spawning rate
- Good for general testing

### Stress Test

- High particle count (1000)
- Aggressive spawning rate
- Tests system performance limits

### Burst Demo

- Periodic particle explosions
- Tests collision handling under high density
- Demonstrates burst spawning

### Rainbow Demo

- Enhanced color variety
- Tests rendering with diverse visuals
- Good for visual stress testing

## Performance Characteristics

The particle system is designed to stress-test various aspects of the Lite2D engine:

- **Entity Management**: Large numbers of entities with multiple components
- **Collision Detection**: O(n²) algorithm for comprehensive testing
- **System Updates**: Multiple systems running every frame
- **Memory Management**: Dynamic entity creation/destruction
- **Rendering Performance**: High-frequency draw calls

## Configuration

Key parameters can be adjusted in `ParticleGame::ConfigureSystems()`:

- **Particle Properties**: Size, mass, velocity, lifetime ranges
- **Physics**: Elasticity, friction coefficients
- **Spawn Behavior**: Auto-spawn intervals, max particles
- **Visual**: Color ranges, rendering properties

## Building

The example is automatically built with the main Lite2D project:

```bash
cd build
make colliding_particles
```

## Running

```bash
./bin/colliding_particles [width] [height]
```

Default window size is 1920x1080. You can specify custom dimensions as command-line arguments.

## Performance Tips

- **Stress Test Mode**: Use mode 2 to push the system to its limits
- **Window Size**: Larger windows may impact performance due to more particles fitting on screen
- **Statistics**: Press 'S' to monitor FPS and collision counts
- **Reset**: Use 'R' to clear particles and reset statistics when testing

This example serves as both a demonstration of the Lite2D engine's capabilities and a comprehensive stress test for system performance and stability.
