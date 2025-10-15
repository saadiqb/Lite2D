#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"

using namespace Lite2D::ECS;

class MovementSystemPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
        
        // Register MovementSystem
        movementSystem = systemManager->RegisterSystem<MovementSystem>();
        
        // Set system signature
        systemManager->SetSystemSignature<MovementSystem>(
            entityManager->GetComponentSignature<Position, Velocity>()
        );
        
        // Configure system
        movementSystem->SetBoundaries(0, 0, 800, 600);
        movementSystem->EnableBoundaryClamping(true);
        movementSystem->SetMaxSpeed(200.0f);
    }
    
    void TearDown() override {
        entityManager.reset();
        systemManager.reset();
        movementSystem.reset();
    }
    
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;
    std::shared_ptr<MovementSystem> movementSystem;
};

// Test 1: Single Entity Performance
TEST_F(MovementSystemPerformanceTest, SingleEntityPerformance) {
    // Create one entity
    Entity entity = entityManager->CreateEntity();
    entityManager->AddComponent(entity, Position(100.0f, 100.0f));
    entityManager->AddComponent(entity, Velocity(50.0f, 30.0f));
    
    // Time 1000 updates
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        movementSystem->Update(*entityManager, 0.016f); // 60 FPS deltaTime
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "\n[SINGLE ENTITY] 1000 updates took: " << duration << "ms" << std::endl;
    std::cout << "[SINGLE ENTITY] Average per update: " << (duration / 1000.0f) << "ms" << std::endl;
    
    // Should be very fast - less than 1ms per update
    EXPECT_LT(duration / 1000.0f, 1.0f) << "Single entity updates should be under 1ms";
}

// Test 2: Multiple Entities Performance
TEST_F(MovementSystemPerformanceTest, MultipleEntitiesPerformance) {
    // Create 100 entities
    std::vector<Entity> entities;
    for (int i = 0; i < 100; ++i) {
        Entity entity = entityManager->CreateEntity();
        entityManager->AddComponent(entity, Position(i * 8.0f, i * 6.0f));
        entityManager->AddComponent(entity, Velocity(i % 50, (i + 1) % 50));
        entities.push_back(entity);
    }
    
    std::cout << "\n[ENTITY COUNT] Created " << entities.size() << " entities" << std::endl;
    
    // Time 100 updates
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; ++i) {
        movementSystem->Update(*entityManager, 0.016f);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "[MULTIPLE ENTITIES] 100 updates with " << entities.size() << " entities took: " << duration << "ms" << std::endl;
    std::cout << "[MULTIPLE ENTITIES] Average per update: " << (duration / 100.0f) << "ms" << std::endl;
    std::cout << "[MULTIPLE ENTITIES] Per entity per update: " << (duration / 100.0f / entities.size()) << "ms" << std::endl;
    
    // Should still be reasonable - under 10ms per update for 100 entities
    EXPECT_LT(duration / 100.0f, 10.0f) << "100 entity updates should be under 10ms";
}

// Test 3: Entity Query Performance
TEST_F(MovementSystemPerformanceTest, EntityQueryPerformance) {
    // Create entities with different component combinations
    Entity movingEntity = entityManager->CreateEntity();
    entityManager->AddComponent(movingEntity, Position(100.0f, 100.0f));
    entityManager->AddComponent(movingEntity, Velocity(50.0f, 30.0f));
    
    Entity staticEntity = entityManager->CreateEntity();
    entityManager->AddComponent(staticEntity, Position(200.0f, 200.0f));
    // No velocity component
    
    Entity renderOnlyEntity = entityManager->CreateEntity();
    entityManager->AddComponent(renderOnlyEntity, Position(300.0f, 300.0f));
    // No velocity component
    
    // Time GetEntitiesWith queries
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        auto entities = entityManager->GetEntitiesWith<Position, Velocity>();
        // This is what MovementSystem does internally
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "\n[ENTITY QUERY] 1000 GetEntitiesWith<Position,Velocity> queries took: " << duration << "ms" << std::endl;
    std::cout << "[ENTITY QUERY] Average per query: " << (duration / 1000.0f) << "ms" << std::endl;
    
    // Entity queries should be very fast
    EXPECT_LT(duration / 1000.0f, 0.1f) << "Entity queries should be under 0.1ms";
}

// Test 4: Component Access Performance
TEST_F(MovementSystemPerformanceTest, ComponentAccessPerformance) {
    // Create entity
    Entity entity = entityManager->CreateEntity();
    entityManager->AddComponent(entity, Position(100.0f, 100.0f));
    entityManager->AddComponent(entity, Velocity(50.0f, 30.0f));
    
    // Time component access
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10000; ++i) {
        Position* pos = entityManager->GetComponent<Position>(entity);
        Velocity* vel = entityManager->GetComponent<Velocity>(entity);
        
        // Simulate what MovementSystem does
        if (pos && vel) {
            pos->x += vel->x * 0.016f;
            pos->y += vel->y * 0.016f;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "\n[COMPONENT ACCESS] 10000 component accesses and updates took: " << duration << "ms" << std::endl;
    std::cout << "[COMPONENT ACCESS] Average per access: " << (duration / 10000.0f) << "ms" << std::endl;
    
    // Component access should be extremely fast
    EXPECT_LT(duration / 10000.0f, 0.01f) << "Component access should be under 0.01ms";
}

// Test 5: Boundary Clamping Performance
TEST_F(MovementSystemPerformanceTest, BoundaryClampingPerformance) {
    // Create entity at boundary
    Entity entity = entityManager->CreateEntity();
    entityManager->AddComponent(entity, Position(790.0f, 590.0f)); // Near boundary
    entityManager->AddComponent(entity, Velocity(100.0f, 100.0f)); // Moving toward boundary
    
    // Time boundary clamping operations
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        movementSystem->Update(*entityManager, 0.016f);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "\n[BOUNDARY CLAMPING] 1000 boundary clamping updates took: " << duration << "ms" << std::endl;
    std::cout << "[BOUNDARY CLAMPING] Average per update: " << (duration / 1000.0f) << "ms" << std::endl;
    
    // Boundary operations should be fast
    EXPECT_LT(duration / 1000.0f, 5.0f) << "Boundary clamping should be under 5ms";
}

// Test 6: System Manager Performance
TEST_F(MovementSystemPerformanceTest, SystemManagerPerformance) {
    // Create entities
    for (int i = 0; i < 10; ++i) {
        Entity entity = entityManager->CreateEntity();
        entityManager->AddComponent(entity, Position(i * 80.0f, i * 60.0f));
        entityManager->AddComponent(entity, Velocity(i * 10.0f, i * 10.0f));
    }
    
    // Time SystemManager.UpdateSystems calls
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        systemManager->UpdateSystems(*entityManager, 0.016f);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start).count();
    
    std::cout << "\n[SYSTEM MANAGER] 1000 SystemManager updates took: " << duration << "ms" << std::endl;
    std::cout << "[SYSTEM MANAGER] Average per update: " << (duration / 1000.0f) << "ms" << std::endl;
    
    // System manager overhead should be minimal
    EXPECT_LT(duration / 1000.0f, 2.0f) << "SystemManager overhead should be under 2ms";
}
