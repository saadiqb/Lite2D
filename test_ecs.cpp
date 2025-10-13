#include <iostream>
#include "ECS/EntityManager.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"

using namespace Lite2D::ECS;

int main() {
    std::cout << "Testing Lite2D ECS System..." << std::endl;
    
    // Create entity manager
    EntityManager entityManager;
    
    // Create some entities
    Entity player = entityManager.CreateEntity();
    Entity enemy = entityManager.CreateEntity();
    Entity item = entityManager.CreateEntity();
    
    std::cout << "Created entities: " << player << ", " << enemy << ", " << item << std::endl;
    
    // Add components to player
    entityManager.AddComponent(player, Position(100.0f, 200.0f));
    entityManager.AddComponent(player, Velocity(5.0f, 0.0f));
    entityManager.AddComponent(player, Renderable(true, 1));
    
    // Add components to enemy
    entityManager.AddComponent(enemy, Position(300.0f, 150.0f));
    entityManager.AddComponent(enemy, Velocity(-2.0f, 1.0f));
    entityManager.AddComponent(enemy, Renderable(true, 0));
    
    // Add components to item (no velocity)
    entityManager.AddComponent(item, Position(50.0f, 50.0f));
    entityManager.AddComponent(item, Renderable(true, 2));
    
    std::cout << "Added components to entities" << std::endl;
    
    // Test component retrieval
    Position* playerPos = entityManager.GetComponent<Position>(player);
    if (playerPos) {
        std::cout << "Player position: (" << playerPos->x << ", " << playerPos->y << ")" << std::endl;
    }
    
    Velocity* enemyVel = entityManager.GetComponent<Velocity>(enemy);
    if (enemyVel) {
        std::cout << "Enemy velocity: (" << enemyVel->x << ", " << enemyVel->y << ")" << std::endl;
    }
    
    // Test component existence
    std::cout << "Player has Position: " << entityManager.HasComponent<Position>(player) << std::endl;
    std::cout << "Item has Velocity: " << entityManager.HasComponent<Velocity>(item) << std::endl;
    
    // Test entity queries
    auto entitiesWithPosition = entityManager.GetEntitiesWith<Position>();
    std::cout << "Entities with Position: " << entitiesWithPosition.size() << std::endl;
    
    auto entitiesWithPositionAndVelocity = entityManager.GetEntitiesWith<Position, Velocity>();
    std::cout << "Entities with Position and Velocity: " << entitiesWithPositionAndVelocity.size() << std::endl;
    
    auto entitiesWithAll = entityManager.GetEntitiesWith<Position, Velocity, Renderable>();
    std::cout << "Entities with all components: " << entitiesWithAll.size() << std::endl;
    
    // Test entity destruction
    entityManager.DestroyEntity(enemy);
    std::cout << "Destroyed enemy entity" << std::endl;
    
    auto entitiesWithPositionAfter = entityManager.GetEntitiesWith<Position>();
    std::cout << "Entities with Position after destruction: " << entitiesWithPositionAfter.size() << std::endl;
    
    std::cout << "ECS test completed successfully!" << std::endl;
    
    return 0;
}
