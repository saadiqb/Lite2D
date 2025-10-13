#include "GameLogicSystem.h"
#include "../../ECS/Components/Position.h"
#include "../../ECS/Components/Renderable.h"
#include "../Components/SnakeHead.h"
#include "../Components/SnakeSegment.h"
#include "../Components/Food.h"
#include "../Components/Wall.h"
#include <iostream>

namespace Lite2D {
namespace ECS {

void GameLogicSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled || mGameStateEntity == INVALID_ENTITY) return;
    
    GameState* gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    if (!gameState) return;
    
    // Handle different game states
    switch (gameState->currentState) {
        case GameState::MENU:
            // Menu logic (could show menu, wait for input)
            break;
            
        case GameState::PLAYING:
            CheckLevelProgression(entityManager);
            break;
            
        case GameState::PAUSED:
            // Paused logic
            break;
            
        case GameState::GAME_OVER:
            // Game over logic
            break;
    }
}

void GameLogicSystem::Initialize(EntityManager& entityManager) {
    std::cout << "GameLogicSystem initialized" << std::endl;
}

void GameLogicSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "GameLogicSystem shutdown" << std::endl;
}

void GameLogicSystem::StartNewGame(EntityManager& entityManager) {
    // Clear existing game entities
    ResetGame(entityManager);
    
    // Initialize new game
    InitializeSnake(entityManager);
    InitializeFood(entityManager);
    InitializeWalls(entityManager);
    
    // Set game state
    GameState* gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    if (gameState) {
        gameState->currentState = GameState::PLAYING;
        std::cout << "New game started!" << std::endl;
    }
}

void GameLogicSystem::ResetGame(EntityManager& entityManager) {
    // Destroy all snake segments
    auto segments = entityManager.GetEntitiesWith<SnakeSegment>();
    for (Entity segment : segments) {
        entityManager.DestroyEntity(segment);
    }
    
    // Destroy all food
    auto foodEntities = entityManager.GetEntitiesWith<Food>();
    for (Entity food : foodEntities) {
        entityManager.DestroyEntity(food);
    }
    
    // Destroy all walls
    auto wallEntities = entityManager.GetEntitiesWith<Wall>();
    for (Entity wall : wallEntities) {
        entityManager.DestroyEntity(wall);
    }
    
    // Reset game state
    GameState* gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    if (gameState) {
        gameState->ResetGame();
    }
}

void GameLogicSystem::PauseGame() {
    if (mGameStateEntity == INVALID_ENTITY) return;
    
    // Note: This method needs entityManager parameter to work properly
    // For now, we'll handle pause/resume through the input system
}

void GameLogicSystem::ResumeGame() {
    if (mGameStateEntity == INVALID_ENTITY) return;
    
    // Note: This method needs entityManager parameter to work properly
    // For now, we'll handle pause/resume through the input system
}

void GameLogicSystem::InitializeSnake(EntityManager& entityManager) {
    // Create snake head
    mSnakeHeadEntity = entityManager.CreateEntity();
    entityManager.AddComponent(mSnakeHeadEntity, Position(200.0f, 200.0f));
    entityManager.AddComponent(mSnakeHeadEntity, Renderable(true, 1)); // Head on layer 1
    entityManager.AddComponent(mSnakeHeadEntity, SnakeHead(SnakeHead::RIGHT, INITIAL_MOVE_INTERVAL));
    
    // Create initial snake body
    for (int i = 1; i <= INITIAL_SNAKE_LENGTH; ++i) {
        Entity segment = entityManager.CreateEntity();
        entityManager.AddComponent(segment, Position(200.0f - (i * GRID_SIZE), 200.0f));
        entityManager.AddComponent(segment, Renderable(true, 0)); // Body on layer 0
        entityManager.AddComponent(segment, SnakeSegment(i));
    }
    
    std::cout << "Snake initialized with " << INITIAL_SNAKE_LENGTH + 1 << " segments" << std::endl;
}

void GameLogicSystem::InitializeFood(EntityManager& entityManager) {
    // Create initial food
    Entity foodEntity = entityManager.CreateEntity();
    entityManager.AddComponent(foodEntity, Position(400.0f, 300.0f));
    entityManager.AddComponent(foodEntity, Renderable(true, 2)); // Food on layer 2
    entityManager.AddComponent(foodEntity, Food(10, true));
    
    std::cout << "Food initialized" << std::endl;
}

void GameLogicSystem::InitializeWalls(EntityManager& entityManager) {
    // For now, we'll rely on boundary checking instead of creating wall entities
    // In a more complex version, you could create actual wall entities here
    
    std::cout << "Walls initialized (boundary checking enabled)" << std::endl;
}

void GameLogicSystem::CheckLevelProgression(EntityManager& entityManager) {
    GameState* gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    if (!gameState) return;
    
    // Level up every 50 points
    int targetScore = gameState->level * SCORE_PER_LEVEL;
    if (gameState->score >= targetScore) {
        gameState->NextLevel();
        
        // Increase snake speed
        SnakeHead* head = entityManager.GetComponent<SnakeHead>(mSnakeHeadEntity);
        if (head) {
            head->moveInterval *= 0.9f; // 10% faster
            if (head->moveInterval < 0.05f) {
                head->moveInterval = 0.05f; // Minimum speed
            }
        }
        
        std::cout << "Level up! Now at level " << gameState->level 
                  << " (Score: " << gameState->score << ")" << std::endl;
    }
}

} // namespace ECS
} // namespace Lite2D
