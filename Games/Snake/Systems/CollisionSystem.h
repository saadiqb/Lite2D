#pragma once

#include "../../ECS/System.h"
#include "../../ECS/EntityManager.h"
#include "../Components/SnakeHead.h"
#include "../Components/Food.h"
#include "../Components/Wall.h"
#include "../Components/GameState.h"

namespace Lite2D {
namespace ECS {

/**
 * Collision System for Snake Game
 * Handles collisions between snake, food, walls, and self
 */
class CollisionSystem : public System {
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "CollisionSystem"; }
    
    // Collision detection
    void SetSnakeHeadEntity(Entity snakeHead) { mSnakeHeadEntity = snakeHead; }
    void SetGameStateEntity(Entity gameStateEntity) { mGameStateEntity = gameStateEntity; }
    
    // Game boundaries
    void SetBoundaries(float minX, float minY, float maxX, float maxY);

private:
    Entity mSnakeHeadEntity = INVALID_ENTITY;
    Entity mGameStateEntity = INVALID_ENTITY;
    
    float mMinX = 0.0f, mMinY = 0.0f, mMaxX = 800.0f, mMaxY = 600.0f;
    
    // Collision detection helpers
    bool CheckSnakeFoodCollision(EntityManager& entityManager);
    bool CheckSnakeWallCollision(EntityManager& entityManager);
    bool CheckSnakeSelfCollision(EntityManager& entityManager);
    bool CheckSnakeBoundaryCollision(EntityManager& entityManager);
    
    // Collision response
    void HandleFoodCollision(EntityManager& entityManager, Entity foodEntity);
    void HandleWallCollision(EntityManager& entityManager);
    void HandleSelfCollision(EntityManager& entityManager);
    void HandleBoundaryCollision(EntityManager& entityManager);
    
    // Utility
    bool IsPositionEqual(float x1, float y1, float x2, float y2, float tolerance = 1.0f);
    void SpawnNewFood(EntityManager& entityManager);
    float GetRandomPosition(float min, float max, float gridSize);
};

} // namespace ECS
} // namespace Lite2D
