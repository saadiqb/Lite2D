#!/bin/bash

# Lite2D ECS Test Runner
# This script builds and runs the ECS unit tests

set -e  # Exit on any error

echo "=== Lite2D ECS Test Runner ==="
echo ""

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: Please run this script from the project root directory"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

echo "Building ECS tests..."
make ecs_tests

echo ""
echo "Running ECS tests..."
echo "===================="

# Run the tests
if [ -f "bin/ecs_tests" ]; then
    ./bin/ecs_tests --gtest_output=xml:test_results.xml
    echo ""
    echo "Test results saved to build/test_results.xml"
else
    echo "Error: Test executable not found"
    exit 1
fi

echo ""
echo "Running tests with CTest..."
ctest --output-on-failure

echo ""
echo "=== All tests completed ==="
