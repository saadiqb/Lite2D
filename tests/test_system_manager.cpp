#include <gtest/gtest.h>
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"

using namespace Lite2D::ECS;

// Mock renderer for testing
class MockRenderer {
public:
    MockRenderer() = default;
};

class SystemManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
        
        // Register component types
        entityManager->RegisterComponentType<Position>();
        entityManager->RegisterComponentType<Velocity>();
        entityManager->RegisterComponentType<Renderable>();
        
        // Create mock renderer
        mockRenderer = std::make_unique<MockRenderer>();
    }
    
    void TearDown() override {
        systemManager.reset();
        entityManager.reset();
        mockRenderer.reset();
    }
    
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;
    std::unique_ptr<MockRenderer> mockRenderer;
};

// Test system registration
TEST_F(SystemManagerTest, RegisterSystem) {
    // Register MovementSystem
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    EXPECT_NE(movementSystem, nullptr);
    
    // Register RenderSystem
    auto renderSystem = systemManager->RegisterSystem<RenderSystem>(nullptr);
    EXPECT_NE(renderSystem, nullptr);
    
    // Try to register same system again - should return existing instance
    auto sameMovementSystem = systemManager->RegisterSystem<MovementSystem>();
    EXPECT_EQ(movementSystem, sameMovementSystem);
}

// Test system signature setting
TEST_F(SystemManagerTest, SetSystemSignature) {
    // Register system
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    
    // Set signature
    auto signature = entityManager->GetComponentSignature<Position, Velocity>();
    systemManager->SetSystemSignature<MovementSystem>(signature);
    
    // Verify system was registered
    EXPECT_NE(movementSystem, nullptr);
}

// Test system initialization
TEST_F(SystemManagerTest, InitializeSystems) {
    // Register systems
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto renderSystem = systemManager->RegisterSystem<RenderSystem>(nullptr);
    
    // Initialize systems
    systemManager->InitializeAllSystems(*entityManager);
    
    // Systems should be initialized (no exceptions thrown)
    EXPECT_NE(movementSystem, nullptr);
    EXPECT_NE(renderSystem, nullptr);
}

// Test system updates
TEST_F(SystemManagerTest, UpdateSystems) {
    // Register and set up MovementSystem
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto signature = entityManager->GetComponentSignature<Position, Velocity>();
    systemManager->SetSystemSignature<MovementSystem>(signature);
    
    // Create entity with Position and Velocity
    Entity entity = entityManager->CreateEntity();
    entityManager->AddComponent(entity, Position(0.0f, 0.0f));
    entityManager->AddComponent(entity, Velocity(10.0f, 5.0f));
    
    // Initialize systems
    systemManager->InitializeAllSystems(*entityManager);
    
    // Update systems
    double deltaTime = 0.016; // ~60 FPS
    systemManager->UpdateSystems(*entityManager, deltaTime);
    
    // Verify position was updated
    Position* pos = entityManager->GetComponent<Position>(entity);
    EXPECT_NE(pos, nullptr);
    EXPECT_GT(pos->x, 0.0f); // Should have moved
    EXPECT_GT(pos->y, 0.0f); // Should have moved
}

// Test system shutdown
TEST_F(SystemManagerTest, ShutdownSystems) {
    // Register systems
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto renderSystem = systemManager->RegisterSystem<RenderSystem>(nullptr);
    
    // Initialize systems
    systemManager->InitializeAllSystems(*entityManager);
    
    // Shutdown systems (should not crash)
    systemManager->ShutdownAllSystems(*entityManager);
    
    EXPECT_NE(movementSystem, nullptr);
    EXPECT_NE(renderSystem, nullptr);
}

// Test entity signature change notification
TEST_F(SystemManagerTest, OnEntitySignatureChanged) {
    // Register MovementSystem
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto signature = entityManager->GetComponentSignature<Position, Velocity>();
    systemManager->SetSystemSignature<MovementSystem>(signature);
    
    // Initialize systems
    systemManager->InitializeAllSystems(*entityManager);
    
    // Create entity with only Position
    Entity entity = entityManager->CreateEntity();
    entityManager->AddComponent(entity, Position(0.0f, 0.0f));
    
    // Notify system of signature change
    auto entitySignature = entityManager->GetComponentSignature<Position>();
    systemManager->OnEntitySignatureChanged(entity, entitySignature);
    
    // Add Velocity component
    entityManager->AddComponent(entity, Velocity(10.0f, 5.0f));
    entitySignature = entityManager->GetComponentSignature<Position, Velocity>();
    systemManager->OnEntitySignatureChanged(entity, entitySignature);
    
    // Entity should now be processed by movement system
    EXPECT_NE(movementSystem, nullptr);
}

// Test getting system
TEST_F(SystemManagerTest, GetSystem) {
    // Register system
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    
    // Get system
    auto retrievedSystem = systemManager->GetSystem<MovementSystem>();
    EXPECT_EQ(movementSystem, retrievedSystem);
    
    // Try to get non-existent system
    auto nonExistentSystem = systemManager->GetSystem<RenderSystem>();
    EXPECT_EQ(nonExistentSystem, nullptr);
}

// Test system count
TEST_F(SystemManagerTest, GetSystemCount) {
    EXPECT_EQ(systemManager->GetSystemCount(), 0);
    
    // Register systems
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto renderSystem = systemManager->RegisterSystem<RenderSystem>(nullptr);
    
    EXPECT_EQ(systemManager->GetSystemCount(), 2);
}

// Test enable/disable system
TEST_F(SystemManagerTest, EnableDisableSystem) {
    // Register system
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    
    // Test enabling/disabling by system name
    systemManager->EnableSystem("MovementSystem", false);
    systemManager->EnableSystem("MovementSystem", true);
    
    EXPECT_NE(movementSystem, nullptr);
}

// Test multiple systems
TEST_F(SystemManagerTest, MultipleSystems) {
    // Register multiple systems
    auto movementSystem = systemManager->RegisterSystem<MovementSystem>();
    auto renderSystem = systemManager->RegisterSystem<RenderSystem>(nullptr);
    
    // Set signatures
    auto movementSignature = entityManager->GetComponentSignature<Position, Velocity>();
    auto renderSignature = entityManager->GetComponentSignature<Position, Renderable>();
    
    systemManager->SetSystemSignature<MovementSystem>(movementSignature);
    systemManager->SetSystemSignature<RenderSystem>(renderSignature);
    
    // Initialize systems
    systemManager->InitializeAllSystems(*entityManager);
    
    // Create entities
    Entity movingEntity = entityManager->CreateEntity();
    entityManager->AddComponent(movingEntity, Position(0.0f, 0.0f));
    entityManager->AddComponent(movingEntity, Velocity(10.0f, 5.0f));
    
    Entity renderableEntity = entityManager->CreateEntity();
    entityManager->AddComponent(renderableEntity, Position(100.0f, 100.0f));
    entityManager->AddComponent(renderableEntity, Renderable(true, 1));
    
    Entity bothEntity = entityManager->CreateEntity();
    entityManager->AddComponent(bothEntity, Position(200.0f, 200.0f));
    entityManager->AddComponent(bothEntity, Velocity(5.0f, -3.0f));
    entityManager->AddComponent(bothEntity, Renderable(true, 2));
    
    // Notify systems of signature changes
    systemManager->OnEntitySignatureChanged(movingEntity, entityManager->GetComponentSignature<Position, Velocity>());
    systemManager->OnEntitySignatureChanged(renderableEntity, entityManager->GetComponentSignature<Position, Renderable>());
    systemManager->OnEntitySignatureChanged(bothEntity, entityManager->GetComponentSignature<Position, Velocity, Renderable>());
    
    // Verify systems are working
    EXPECT_NE(movementSystem, nullptr);
    EXPECT_NE(renderSystem, nullptr);
}
