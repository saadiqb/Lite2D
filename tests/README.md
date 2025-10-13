# Lite2D ECS Unit Tests

This directory contains comprehensive unit tests for the Lite2D ECS (Entity-Component-System) game engine.

## Test Structure

### Test Files

- **`test_entity_manager.cpp`** - Tests for EntityManager functionality

  - Entity creation and destruction
  - Component addition, removal, and retrieval
  - Entity queries and signatures
  - Entity pool recycling

- **`test_component_array.cpp`** - Tests for ComponentArray functionality

  - Component storage and retrieval
  - Component updates and removal
  - Entity destruction handling
  - Array density maintenance

- **`test_system_manager.cpp`** - Tests for SystemManager functionality

  - System registration and management
  - System signature setting
  - System enable/disable functionality
  - Entity signature change notifications

- **`test_systems.cpp`** - Tests for individual ECS Systems

  - MovementSystem functionality
  - RenderSystem functionality
  - System updates and entity processing
  - Boundary enforcement

- **`test_integration.cpp`** - Integration tests for complete ECS workflows
  - End-to-end ECS operations
  - Dynamic component addition/removal
  - Performance tests with many entities
  - Complex entity queries

## Building and Running Tests

### Prerequisites

- CMake 3.16 or higher
- Google Test (GTest) library
- C++17 compatible compiler

### Building

```bash
# From the project root directory
mkdir -p build
cd build
cmake ..
make ecs_tests
```

### Running Tests

```bash
# Run all tests
./bin/ecs_tests

# Run with verbose output
./bin/ecs_tests --gtest_verbose

# Run specific test suite
./bin/ecs_tests --gtest_filter="EntityManagerTest.*"

# Run specific test
./bin/ecs_tests --gtest_filter="EntityManagerTest.CreateEntity"
```

### Using CTest

```bash
# Run tests using CTest
ctest

# Run with verbose output
ctest --verbose

# Run specific test
ctest -R ECS_Tests
```

## Test Coverage

The test suite covers:

### EntityManager

- ✅ Entity lifecycle (create, destroy, validate)
- ✅ Component management (add, remove, get, has)
- ✅ Entity queries (GetEntitiesWith)
- ✅ Component signatures
- ✅ Entity pool recycling
- ✅ Error handling (invalid entities)

### ComponentArray

- ✅ Component storage and retrieval
- ✅ Component updates and removal
- ✅ Entity destruction handling
- ✅ Array density maintenance
- ✅ Type safety

### SystemManager

- ✅ System registration and retrieval
- ✅ System signature management
- ✅ System enable/disable
- ✅ Entity signature change notifications
- ✅ System lifecycle (init, update, shutdown)

### Systems

- ✅ MovementSystem position updates
- ✅ MovementSystem boundary enforcement
- ✅ RenderSystem entity management
- ✅ System entity filtering
- ✅ System updates with no entities

### Integration

- ✅ Complete ECS workflows
- ✅ Dynamic component operations
- ✅ Entity destruction
- ✅ System enable/disable
- ✅ Performance with many entities
- ✅ Complex entity queries

## Test Philosophy

The tests follow these principles:

1. **Isolation** - Each test is independent and can run in any order
2. **Completeness** - Cover both success and failure cases
3. **Performance** - Include performance tests for critical paths
4. **Integration** - Test complete workflows, not just individual components
5. **Maintainability** - Clear test names and structure for easy maintenance

## Adding New Tests

When adding new features to the ECS system:

1. Add unit tests for new functionality
2. Update integration tests if needed
3. Ensure all tests pass
4. Update this README if new test categories are added

## Continuous Integration

These tests are designed to be run in CI/CD pipelines:

```bash
# CI-friendly test run
ctest --output-on-failure --no-compress-output
```
