#include "SnakeMovementSystem.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Renderable.h"
#include <algorithm>
#include <iostream>

namespace Lite2D {
namespace ECS {

void SnakeMovementSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled || mSnakeHeadEntity == INVALID_ENTITY) return;
    
    SnakeHead* head = entityManager.GetComponent<SnakeHead>(mSnakeHeadEntity);
    if (!head) return;
    
    // Check if game is in playing state
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
        if (gameState && gameState->currentState != GameState::PLAYING) {
            return;
        }
    }
    
    // Update movement timer
    head->moveTimer += deltaTime;
    
    // Apply game speed multiplier
    float moveInterval = head->moveInterval;
    if (gameState) {
        moveInterval /= gameState->gameSpeed;
    }
    
    // Check if it's time to move
    if (head->moveTimer >= moveInterval) {
        head->moveTimer = 0.0f;
        
        // Update direction
        head->currentDirection = head->nextDirection;
        
        // Store current head position for body segments
        Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
        if (headPos) {
            PositionHistory history;
            history.x = headPos->x;
            history.y = headPos->y;
            history.timestamp = head->moveTimer;
            mPositionHistory.push_back(history);
            
            // Limit history size
            if (mPositionHistory.size() > MAX_HISTORY_SIZE) {
                mPositionHistory.erase(mPositionHistory.begin());
            }
        }
        
        // Move the snake
        MoveSnakeHead(entityManager, head);
        MoveSnakeBody(entityManager, head);
        
        // Add new segments if needed
        while (head->segmentsToAdd > 0) {
            // Find the last segment position to add new segment
            if (!mPositionHistory.empty()) {
                auto lastPos = mPositionHistory.back();
                AddSnakeSegment(entityManager, lastPos.x, lastPos.y, -1);
                head->segmentsToAdd--;
            } else {
                break;
            }
        }
    }
}

void SnakeMovementSystem::Initialize(EntityManager& entityManager) {
    std::cout << "SnakeMovementSystem initialized" << std::endl;
}

void SnakeMovementSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "SnakeMovementSystem shutdown" << std::endl;
}

void SnakeMovementSystem::MoveSnakeHead(EntityManager& entityManager, SnakeHead* head) {
    Position* headPos = entityManager.GetComponent<Position>(mSnakeHeadEntity);
    if (!headPos) return;
    
    float dirX, dirY;
    head->GetDirectionVector(dirX, dirY);
    
    // Move head by grid size (assuming 20x20 grid)
    const float GRID_SIZE = 20.0f;
    headPos->x += dirX * GRID_SIZE;
    headPos->y += dirY * GRID_SIZE;
}

void SnakeMovementSystem::MoveSnakeBody(EntityManager& entityManager, SnakeHead* head) {
    // Get all snake segments
    auto segments = entityManager.GetEntitiesWith<SnakeSegment>();
    
    // Sort segments by index (head = 0, body = 1, 2, 3...)
    std::sort(segments.begin(), segments.end(), 
        [&entityManager](Entity a, Entity b) {
            SnakeSegment* segA = entityManager.GetComponent<SnakeSegment>(a);
            SnakeSegment* segB = entityManager.GetComponent<SnakeSegment>(b);
            if (!segA || !segB) return false;
            return segA->segmentIndex < segB->segmentIndex;
        });
    
    // Move each body segment to the position of the previous segment
    // Use the position history to track where each segment should move
    for (size_t i = 0; i < segments.size() && i < mPositionHistory.size(); ++i) {
        Position* segmentPos = entityManager.GetComponent<Position>(segments[i]);
        if (!segmentPos) continue;
        
        // Each segment follows the position that was recorded 'i' steps ago
        // The most recent position (size()-1) goes to the first segment after head
        // The second most recent (size()-2) goes to the second segment, etc.
        size_t historyIndex = mPositionHistory.size() - 1 - i;
        if (historyIndex < mPositionHistory.size()) {
            segmentPos->x = mPositionHistory[historyIndex].x;
            segmentPos->y = mPositionHistory[historyIndex].y;
        }
    }
}

void SnakeMovementSystem::AddSnakeSegment(EntityManager& entityManager, float x, float y, int segmentIndex) {
    Entity newSegment = entityManager.CreateEntity();
    
    // Add components
    entityManager.AddComponent(newSegment, Position(x, y));
    entityManager.AddComponent(newSegment, Renderable(true, 0)); // Body segments on layer 0
    entityManager.AddComponent(newSegment, SnakeSegment(segmentIndex));
}

void SnakeMovementSystem::GrowSnake(EntityManager& entityManager) {
    if (mSnakeHeadEntity == INVALID_ENTITY) return;
    
    SnakeHead* head = entityManager.GetComponent<SnakeHead>(mSnakeHeadEntity);
    if (!head) return;
    
    head->segmentsToAdd++;
}

} // namespace ECS
} // namespace Lite2D
