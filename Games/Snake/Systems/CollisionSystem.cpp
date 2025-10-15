#include "CollisionSystem.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Renderable.h"
#include "../Components/SnakeSegment.h"
#include <iostream>
#include <random>
#include <cmath>

namespace Lite2D {
namespace ECS {

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled || mSnakeHeadEntity == INVALID_ENTITY) return;
    
    // Check if game is in playing state
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
        if (gameState && gameState->currentState != GameState::PLAYING) {
            return;
        }
    }
    
    // Check for food collision
    if (CheckSnakeFoodCollision(entityManager)) {
        // Food collision is handled in CheckSnakeFoodCollision
        return;
    }
    
    // Check for wall collision
    if (CheckSnakeWallCollision(entityManager)) {
        HandleWallCollision(entityManager);
        return;
    }
    
    // Check for boundary collision
    if (CheckSnakeBoundaryCollision(entityManager)) {
        HandleBoundaryCollision(entityManager);
        return;
    }
    
    // Check for self collision
    if (CheckSnakeSelfCollision(entityManager)) {
        HandleSelfCollision(entityManager);
        return;
    }
}

void CollisionSystem::Initialize(EntityManager& entityManager) {
    std::cout << "CollisionSystem initialized" << std::endl;
}

void CollisionSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "CollisionSystem shutdown" << std::endl;
}

void CollisionSystem::SetBoundaries(float minX, float minY, float maxX, float maxY) {
    mMinX = minX;
    mMinY = minY;
    mMaxX = maxX;
    mMaxY = maxY;
}

bool CollisionSystem::CheckSnakeFoodCollision(EntityManager& entityManager) {
    Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
    if (!headPos) return false;
    
    // Check collision with all food entities
    auto foodEntities = entityManager.GetEntitiesWith<Food>();
    
    for (Entity foodEntity : foodEntities) {
        Food* food = entityManager.GetComponent<Food>(foodEntity);
        Position* foodPos = entityManager.GetComponent<Position>(foodEntity);
        
        if (!food || !foodPos || !food->isActive) continue;
        
        if (IsPositionEqual(headPos->x, headPos->y, foodPos->x, foodPos->y)) {
            HandleFoodCollision(entityManager, foodEntity);
            return true;
        }
    }
    
    return false;
}

bool CollisionSystem::CheckSnakeWallCollision(EntityManager& entityManager) {
    Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
    if (!headPos) return false;
    
    // Check collision with all wall entities
    auto wallEntities = entityManager.GetEntitiesWith<Wall>();
    
    for (Entity wallEntity : wallEntities) {
        Position* wallPos = entityManager.GetComponent<Position>(wallEntity);
        
        if (!wallPos) continue;
        
        if (IsPositionEqual(headPos->x, headPos->y, wallPos->x, wallPos->y)) {
            return true;
        }
    }
    
    return false;
}

bool CollisionSystem::CheckSnakeSelfCollision(EntityManager& entityManager) {
    Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
    if (!headPos) return false;
    
    // Check collision with snake body segments (not head)
    auto segments = entityManager.GetEntitiesWith<SnakeSegment>();
    
    for (Entity segment : segments) {
        SnakeSegment* snakeSegment = entityManager.GetComponent<SnakeSegment>(segment);
        Position* segmentPos = entityManager.GetComponent<Position>(segment);
        
        if (!snakeSegment || !segmentPos) continue;
        
        // Skip the head (segment index 0 is reserved for head)
        if (snakeSegment->segmentIndex <= 0) continue;
        
        if (IsPositionEqual(headPos->x, headPos->y, segmentPos->x, segmentPos->y)) {
            return true;
        }
    }
    
    return false;
}

bool CollisionSystem::CheckSnakeBoundaryCollision(EntityManager& entityManager) {
    Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
    if (!headPos) return false;
    
    // Check if head is outside boundaries
    return (headPos->x < mMinX || headPos->x >= mMaxX || 
            headPos->y < mMinY || headPos->y >= mMaxY);
}

void CollisionSystem::HandleFoodCollision(EntityManager& entityManager, Entity foodEntity) {
    Food* food = entityManager.GetComponent<Food>(foodEntity);
    GameState* gameState = nullptr;
    
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    }
    
    if (food && gameState) {
        // Add score
        gameState->AddScore(food->points);
        
        // Deactivate food
        food->isActive = false;
        
        // Tell snake movement system to grow
        // This would typically be done through a message/event system
        // For now, we'll directly call the grow function
        // In a real implementation, you'd use an event bus
        
        // Destroy the food entity
        entityManager.DestroyEntity(foodEntity);
        
        // Spawn new food
        SpawnNewFood(entityManager);
        
        std::cout << "Snake ate food! Score: " << gameState->score << std::endl;
    }
}

void CollisionSystem::HandleWallCollision(EntityManager& entityManager) {
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    }
    
    if (gameState) {
        gameState->currentState = GameState::GAME_OVER;
        std::cout << "Game Over! Snake hit a wall. Final Score: " << gameState->score << std::endl;
    }
}

void CollisionSystem::HandleSelfCollision(EntityManager& entityManager) {
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    }
    
    if (gameState) {
        gameState->currentState = GameState::GAME_OVER;
        std::cout << "Game Over! Snake hit itself. Final Score: " << gameState->score << std::endl;
    }
}

void CollisionSystem::HandleBoundaryCollision(EntityManager& entityManager) {
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    }
    
    if (gameState) {
        gameState->currentState = GameState::GAME_OVER;
        std::cout << "Game Over! Snake hit boundary. Final Score: " << gameState->score << std::endl;
    }
}

bool CollisionSystem::IsPositionEqual(float x1, float y1, float x2, float y2, float tolerance) {
    return (std::abs(x1 - x2) < tolerance && std::abs(y1 - y2) < tolerance);
}

void CollisionSystem::SpawnNewFood(EntityManager& entityManager) {
    // Create new food entity
    Entity foodEntity = entityManager.CreateEntity();
    
    // Generate random position within boundaries
    const float GRID_SIZE = 20.0f;
    float foodX = GetRandomPosition(mMinX + GRID_SIZE, mMaxX - GRID_SIZE, GRID_SIZE);
    float foodY = GetRandomPosition(mMinY + GRID_SIZE, mMaxY - GRID_SIZE, GRID_SIZE);
    
    // Add components
    entityManager.AddComponent(foodEntity, Position(foodX, foodY));
    entityManager.AddComponent(foodEntity, Renderable(true, 2)); // Food on layer 2
    entityManager.AddComponent(foodEntity, Food(10, true));
    
    std::cout << "New food spawned at (" << foodX << ", " << foodY << ")" << std::endl;
}

float CollisionSystem::GetRandomPosition(float min, float max, float gridSize) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    int steps = static_cast<int>((max - min) / gridSize);
    std::uniform_int_distribution<> dis(0, steps - 1);
    
    return min + (dis(gen) * gridSize);
}

} // namespace ECS
} // namespace Lite2D
