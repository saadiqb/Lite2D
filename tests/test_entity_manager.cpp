#include <gtest/gtest.h>
#include "ECS/EntityManager.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"

using namespace Lite2D::ECS;

class EntityManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        entityManager = std::make_unique<EntityManager>();
        
        // Register component types
        entityManager->RegisterComponentType<Position>();
        entityManager->RegisterComponentType<Velocity>();
        entityManager->RegisterComponentType<Renderable>();
    }
    
    void TearDown() override {
        entityManager.reset();
    }
    
    std::unique_ptr<EntityManager> entityManager;
};

// Test entity creation
TEST_F(EntityManagerTest, CreateEntity) {
    Entity entity = entityManager->CreateEntity();
    
    EXPECT_NE(entity, INVALID_ENTITY);
    EXPECT_TRUE(entityManager->IsValid(entity));
    EXPECT_EQ(entityManager->GetEntityCount(), 1);
}

// Test multiple entity creation
TEST_F(EntityManagerTest, CreateMultipleEntities) {
    std::vector<Entity> entities;
    
    for (int i = 0; i < 10; ++i) {
        Entity entity = entityManager->CreateEntity();
        entities.push_back(entity);
        EXPECT_NE(entity, INVALID_ENTITY);
        EXPECT_TRUE(entityManager->IsValid(entity));
    }
    
    EXPECT_EQ(entityManager->GetEntityCount(), 10);
    
    // Verify all entities are unique
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            EXPECT_NE(entities[i], entities[j]);
        }
    }
}

// Test entity destruction
TEST_F(EntityManagerTest, DestroyEntity) {
    Entity entity = entityManager->CreateEntity();
    EXPECT_EQ(entityManager->GetEntityCount(), 1);
    
    entityManager->DestroyEntity(entity);
    EXPECT_EQ(entityManager->GetEntityCount(), 0);
    EXPECT_FALSE(entityManager->IsValid(entity));
}

// Test component addition
TEST_F(EntityManagerTest, AddComponent) {
    Entity entity = entityManager->CreateEntity();
    
    // Add components
    entityManager->AddComponent(entity, Position(10.0f, 20.0f));
    entityManager->AddComponent(entity, Velocity(5.0f, -3.0f));
    entityManager->AddComponent(entity, Renderable(true, 1));
    
    // Verify components exist
    EXPECT_TRUE(entityManager->HasComponent<Position>(entity));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
    EXPECT_TRUE(entityManager->HasComponent<Renderable>(entity));
    
    // Verify component data
    Position* pos = entityManager->GetComponent<Position>(entity);
    EXPECT_NE(pos, nullptr);
    EXPECT_FLOAT_EQ(pos->x, 10.0f);
    EXPECT_FLOAT_EQ(pos->y, 20.0f);
    
    Velocity* vel = entityManager->GetComponent<Velocity>(entity);
    EXPECT_NE(vel, nullptr);
    EXPECT_FLOAT_EQ(vel->x, 5.0f);
    EXPECT_FLOAT_EQ(vel->y, -3.0f);
    
    Renderable* renderable = entityManager->GetComponent<Renderable>(entity);
    EXPECT_NE(renderable, nullptr);
    EXPECT_TRUE(renderable->visible);
    EXPECT_EQ(renderable->layer, 1);
}

// Test component removal
TEST_F(EntityManagerTest, RemoveComponent) {
    Entity entity = entityManager->CreateEntity();
    
    // Add components
    entityManager->AddComponent(entity, Position(10.0f, 20.0f));
    entityManager->AddComponent(entity, Velocity(5.0f, -3.0f));
    
    EXPECT_TRUE(entityManager->HasComponent<Position>(entity));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
    
    // Remove one component
    entityManager->RemoveComponent<Position>(entity);
    
    EXPECT_FALSE(entityManager->HasComponent<Position>(entity));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
    EXPECT_EQ(entityManager->GetComponent<Position>(entity), nullptr);
}

// Test component update
TEST_F(EntityManagerTest, UpdateComponent) {
    Entity entity = entityManager->CreateEntity();
    
    // Add component
    entityManager->AddComponent(entity, Position(10.0f, 20.0f));
    
    Position* pos = entityManager->GetComponent<Position>(entity);
    EXPECT_FLOAT_EQ(pos->x, 10.0f);
    EXPECT_FLOAT_EQ(pos->y, 20.0f);
    
    // Update component
    entityManager->AddComponent(entity, Position(30.0f, 40.0f));
    
    pos = entityManager->GetComponent<Position>(entity);
    EXPECT_FLOAT_EQ(pos->x, 30.0f);
    EXPECT_FLOAT_EQ(pos->y, 40.0f);
}

// Test GetEntitiesWith query
TEST_F(EntityManagerTest, GetEntitiesWith) {
    // Create entities with different component combinations
    Entity entity1 = entityManager->CreateEntity();
    entityManager->AddComponent(entity1, Position(10.0f, 20.0f));
    entityManager->AddComponent(entity1, Velocity(5.0f, -3.0f));
    
    Entity entity2 = entityManager->CreateEntity();
    entityManager->AddComponent(entity2, Position(30.0f, 40.0f));
    entityManager->AddComponent(entity2, Renderable(true, 1));
    
    Entity entity3 = entityManager->CreateEntity();
    entityManager->AddComponent(entity3, Position(50.0f, 60.0f));
    entityManager->AddComponent(entity3, Velocity(1.0f, 2.0f));
    entityManager->AddComponent(entity3, Renderable(true, 2));
    
    // Query entities with Position only
    auto entitiesWithPosition = entityManager->GetEntitiesWith<Position>();
    EXPECT_EQ(entitiesWithPosition.size(), 3);
    
    // Query entities with Position and Velocity
    auto entitiesWithPosAndVel = entityManager->GetEntitiesWith<Position, Velocity>();
    EXPECT_EQ(entitiesWithPosAndVel.size(), 2);
    
    // Query entities with all three components
    auto entitiesWithAll = entityManager->GetEntitiesWith<Position, Velocity, Renderable>();
    EXPECT_EQ(entitiesWithAll.size(), 1);
    EXPECT_EQ(entitiesWithAll[0], entity3);
}

// Test entity signature
TEST_F(EntityManagerTest, EntitySignature) {
    Entity entity = entityManager->CreateEntity();
    
    // Test component signature helper
    auto positionSignature = entityManager->GetComponentSignature<Position>();
    auto velocitySignature = entityManager->GetComponentSignature<Velocity>();
    
    EXPECT_FALSE(positionSignature.none());
    EXPECT_FALSE(velocitySignature.none());
    EXPECT_NE(positionSignature, velocitySignature);
    
    // Add components and verify they exist
    entityManager->AddComponent(entity, Position(0.0f, 0.0f));
    EXPECT_TRUE(entityManager->HasComponent<Position>(entity));
    
    entityManager->AddComponent(entity, Velocity(0.0f, 0.0f));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
}

// Test entity destruction with components
TEST_F(EntityManagerTest, DestroyEntityWithComponents) {
    Entity entity = entityManager->CreateEntity();
    
    entityManager->AddComponent(entity, Position(10.0f, 20.0f));
    entityManager->AddComponent(entity, Velocity(5.0f, -3.0f));
    
    EXPECT_TRUE(entityManager->HasComponent<Position>(entity));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
    
    entityManager->DestroyEntity(entity);
    
    EXPECT_FALSE(entityManager->IsValid(entity));
    EXPECT_FALSE(entityManager->HasComponent<Position>(entity));
    EXPECT_FALSE(entityManager->HasComponent<Velocity>(entity));
}

// Test invalid entity operations
TEST_F(EntityManagerTest, InvalidEntityOperations) {
    Entity invalidEntity = INVALID_ENTITY;
    
    EXPECT_FALSE(entityManager->IsValid(invalidEntity));
    EXPECT_FALSE(entityManager->HasComponent<Position>(invalidEntity));
    EXPECT_EQ(entityManager->GetComponent<Position>(invalidEntity), nullptr);
    
    // These operations should not crash
    entityManager->DestroyEntity(invalidEntity);
    entityManager->RemoveComponent<Position>(invalidEntity);
}

// Test entity pool recycling
TEST_F(EntityManagerTest, EntityPoolRecycling) {
    std::vector<Entity> entities;
    
    // Create some entities
    for (int i = 0; i < 5; ++i) {
        Entity entity = entityManager->CreateEntity();
        entities.push_back(entity);
    }
    
    EXPECT_EQ(entityManager->GetEntityCount(), 5);
    
    // Destroy some entities
    entityManager->DestroyEntity(entities[1]);
    entityManager->DestroyEntity(entities[3]);
    
    EXPECT_EQ(entityManager->GetEntityCount(), 3);
    
    // Create new entities - should reuse destroyed entity IDs
    Entity newEntity1 = entityManager->CreateEntity();
    Entity newEntity2 = entityManager->CreateEntity();
    
    EXPECT_EQ(entityManager->GetEntityCount(), 5);
    EXPECT_TRUE(entityManager->IsValid(newEntity1));
    EXPECT_TRUE(entityManager->IsValid(newEntity2));
}

// Test clear operation
TEST_F(EntityManagerTest, Clear) {
    // Create entities and components
    for (int i = 0; i < 5; ++i) {
        Entity entity = entityManager->CreateEntity();
        entityManager->AddComponent(entity, Position(static_cast<float>(i), static_cast<float>(i)));
    }
    
    EXPECT_EQ(entityManager->GetEntityCount(), 5);
    
    entityManager->Clear();
    
    EXPECT_EQ(entityManager->GetEntityCount(), 0);
}

// Test component type registration
TEST_F(EntityManagerTest, ComponentTypeRegistration) {
    Entity entity = entityManager->CreateEntity();
    
    // Position should be registered (done in SetUp)
    entityManager->AddComponent(entity, Position(10.0f, 20.0f));
    EXPECT_TRUE(entityManager->HasComponent<Position>(entity));
    
    // Velocity should be registered (done in SetUp)
    entityManager->AddComponent(entity, Velocity(5.0f, -3.0f));
    EXPECT_TRUE(entityManager->HasComponent<Velocity>(entity));
}
