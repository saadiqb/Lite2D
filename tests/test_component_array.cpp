#include <gtest/gtest.h>
#include "ECS/ComponentArray.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"

using namespace Lite2D::ECS;

class ComponentArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        componentArray = std::make_unique<ComponentArray<Position>>();
    }
    
    void TearDown() override {
        componentArray.reset();
    }
    
    std::unique_ptr<ComponentArray<Position>> componentArray;
};

// Test component array creation
TEST_F(ComponentArrayTest, Creation) {
    EXPECT_NE(componentArray, nullptr);
}

// Test adding components
TEST_F(ComponentArrayTest, AddComponent) {
    Entity entity1 = 1;
    Entity entity2 = 2;
    
    Position pos1(10.0f, 20.0f);
    Position pos2(30.0f, 40.0f);
    
    // Add components
    componentArray->InsertData(entity1, &pos1);
    componentArray->InsertData(entity2, &pos2);
    
    // Verify components were added
    EXPECT_TRUE(componentArray->HasData(entity1));
    EXPECT_TRUE(componentArray->HasData(entity2));
    
    Position* retrieved1 = componentArray->GetComponent(entity1);
    Position* retrieved2 = componentArray->GetComponent(entity2);
    
    EXPECT_NE(retrieved1, nullptr);
    EXPECT_NE(retrieved2, nullptr);
    EXPECT_FLOAT_EQ(retrieved1->x, 10.0f);
    EXPECT_FLOAT_EQ(retrieved1->y, 20.0f);
    EXPECT_FLOAT_EQ(retrieved2->x, 30.0f);
    EXPECT_FLOAT_EQ(retrieved2->y, 40.0f);
}

// Test removing components
TEST_F(ComponentArrayTest, RemoveComponent) {
    Entity entity = 1;
    Position pos(10.0f, 20.0f);
    
    // Add component
    componentArray->InsertData(entity, &pos);
    EXPECT_TRUE(componentArray->HasData(entity));
    
    // Remove component
    componentArray->RemoveData(entity);
    EXPECT_FALSE(componentArray->HasData(entity));
    EXPECT_EQ(componentArray->GetComponent(entity), nullptr);
}

// Test component update
TEST_F(ComponentArrayTest, UpdateComponent) {
    Entity entity = 1;
    Position pos1(10.0f, 20.0f);
    Position pos2(30.0f, 40.0f);
    
    // Add component
    componentArray->InsertData(entity, &pos1);
    
    Position* retrieved = componentArray->GetComponent(entity);
    EXPECT_FLOAT_EQ(retrieved->x, 10.0f);
    EXPECT_FLOAT_EQ(retrieved->y, 20.0f);
    
    // Update component
    componentArray->InsertData(entity, &pos2);
    
    retrieved = componentArray->GetComponent(entity);
    EXPECT_FLOAT_EQ(retrieved->x, 30.0f);
    EXPECT_FLOAT_EQ(retrieved->y, 40.0f);
}

// Test entity destruction
TEST_F(ComponentArrayTest, EntityDestroyed) {
    Entity entity = 1;
    Position pos(10.0f, 20.0f);
    
    // Add component
    componentArray->InsertData(entity, &pos);
    EXPECT_TRUE(componentArray->HasData(entity));
    
    // Simulate entity destruction
    componentArray->EntityDestroyed(entity);
    EXPECT_FALSE(componentArray->HasData(entity));
}

// Test removing non-existent component
TEST_F(ComponentArrayTest, RemoveNonExistentComponent) {
    Entity entity = 1;
    
    // Try to remove component that doesn't exist
    componentArray->RemoveData(entity);
    
    // Should not crash and component should not exist
    EXPECT_FALSE(componentArray->HasData(entity));
}

// Test getting non-existent component
TEST_F(ComponentArrayTest, GetNonExistentComponent) {
    Entity entity = 1;
    
    // Try to get component that doesn't exist
    Position* component = componentArray->GetComponent(entity);
    
    EXPECT_EQ(component, nullptr);
    EXPECT_FALSE(componentArray->HasData(entity));
}

// Test multiple entities
TEST_F(ComponentArrayTest, MultipleEntities) {
    const int numEntities = 10;
    std::vector<Entity> entities;
    
    // Create entities and add components
    for (int i = 0; i < numEntities; ++i) {
        Entity entity = i + 1;
        entities.push_back(entity);
        
        Position pos(static_cast<float>(i), static_cast<float>(i * 2));
        componentArray->InsertData(entity, &pos);
    }
    
    // Verify all components exist
    for (int i = 0; i < numEntities; ++i) {
        EXPECT_TRUE(componentArray->HasData(entities[i]));
        
        Position* component = componentArray->GetComponent(entities[i]);
        EXPECT_NE(component, nullptr);
        EXPECT_FLOAT_EQ(component->x, static_cast<float>(i));
        EXPECT_FLOAT_EQ(component->y, static_cast<float>(i * 2));
    }
}

// Test component removal maintains density
TEST_F(ComponentArrayTest, RemoveMaintainsDensity) {
    const int numEntities = 5;
    std::vector<Entity> entities;
    
    // Add multiple components
    for (int i = 0; i < numEntities; ++i) {
        Entity entity = i + 1;
        entities.push_back(entity);
        Position pos(static_cast<float>(i), static_cast<float>(i));
        componentArray->InsertData(entity, &pos);
    }
    
    // Remove middle component
    Entity middleEntity = entities[2];
    componentArray->RemoveData(middleEntity);
    
    // Verify middle component is gone
    EXPECT_FALSE(componentArray->HasData(middleEntity));
    
    // Verify other components still exist
    for (int i = 0; i < numEntities; ++i) {
        if (i != 2) {
            EXPECT_TRUE(componentArray->HasData(entities[i]));
        }
    }
}

// Test invalid entity
TEST_F(ComponentArrayTest, InvalidEntity) {
    Entity invalidEntity = INVALID_ENTITY;
    
    // Operations with invalid entity should not crash
    Position pos(10.0f, 20.0f);
    componentArray->InsertData(invalidEntity, &pos);
    componentArray->RemoveData(invalidEntity);
    componentArray->EntityDestroyed(invalidEntity);
    
    EXPECT_FALSE(componentArray->HasData(invalidEntity));
    EXPECT_EQ(componentArray->GetComponent(invalidEntity), nullptr);
}

// Test component type name
TEST_F(ComponentArrayTest, ComponentTypeName) {
    const char* typeName = componentArray->GetComponentTypeName();
    EXPECT_NE(typeName, nullptr);
    EXPECT_NE(strlen(typeName), 0);
}

// Test getting all components
TEST_F(ComponentArrayTest, GetComponents) {
    const int numEntities = 3;
    
    // Add components
    for (int i = 0; i < numEntities; ++i) {
        Entity entity = i + 1;
        Position pos(static_cast<float>(i), static_cast<float>(i));
        componentArray->InsertData(entity, &pos);
    }
    
    // Get all components
    Position* components = componentArray->GetComponents();
    EXPECT_NE(components, nullptr);
    
    // Verify we can access the components
    for (int i = 0; i < numEntities; ++i) {
        EXPECT_FLOAT_EQ(components[i].x, static_cast<float>(i));
        EXPECT_FLOAT_EQ(components[i].y, static_cast<float>(i));
    }
}
