#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "../Components/SnakeHead.h"
#include "../Components/SnakeSegment.h"
#include "../Components/GameState.h"

namespace Lite2D {
namespace ECS {

/**
 * Snake Movement System
 * Handles the discrete movement of the snake and body segments
 */
class SnakeMovementSystem : public System {
public:
    SnakeMovementSystem() = default;
    ~SnakeMovementSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "SnakeMovementSystem"; }
    
    // Snake management
    void SetSnakeHeadEntity(Entity snakeHead) { mSnakeHeadEntity = snakeHead; }
    void SetGameStateEntity(Entity gameStateEntity) { mGameStateEntity = gameStateEntity; }
    
    // Snake growth
    void GrowSnake(EntityManager& entityManager);

private:
    Entity mSnakeHeadEntity = INVALID_ENTITY;
    Entity mGameStateEntity = INVALID_ENTITY;
    
    // Movement helpers
    void MoveSnakeHead(EntityManager& entityManager, SnakeHead* head);
    void MoveSnakeBody(EntityManager& entityManager, SnakeHead* head);
    void AddSnakeSegment(EntityManager& entityManager, float x, float y, int segmentIndex);
    
    // Position tracking for body movement
    struct PositionHistory {
        float x, y;
        float timestamp;
    };
    
    std::vector<PositionHistory> mPositionHistory;
    static constexpr int MAX_HISTORY_SIZE = 1000; // Prevent unlimited growth
};

} // namespace ECS
} // namespace Lite2D
