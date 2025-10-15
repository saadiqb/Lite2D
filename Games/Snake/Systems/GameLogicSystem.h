#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "../Components/GameState.h"
#include "../Components/SnakeHead.h"

namespace Lite2D {
namespace ECS {

/**
 * Game Logic System
 * Manages overall game state, scoring, and level progression
 */
class GameLogicSystem : public System {
public:
    GameLogicSystem() = default;
    ~GameLogicSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "GameLogicSystem"; }
    
    // Game management
    void SetGameStateEntity(Entity gameStateEntity) { mGameStateEntity = gameStateEntity; }
    void SetSnakeHeadEntity(Entity snakeHeadEntity) { mSnakeHeadEntity = snakeHeadEntity; }
    
    // Game actions
    void StartNewGame(EntityManager& entityManager);
    void ResetGame(EntityManager& entityManager);
    void PauseGame();
    void ResumeGame();

private:
    Entity mGameStateEntity = INVALID_ENTITY;
    Entity mSnakeHeadEntity = INVALID_ENTITY;
    
    // Game logic helpers
    void InitializeSnake(EntityManager& entityManager);
    void InitializeFood(EntityManager& entityManager);
    void InitializeWalls(EntityManager& entityManager);
    void CheckLevelProgression(EntityManager& entityManager);
    
    // Game configuration
    static constexpr float GRID_SIZE = 20.0f;
    static constexpr float INITIAL_MOVE_INTERVAL = 0.2f;
    static constexpr int INITIAL_SNAKE_LENGTH = 3;
    static constexpr int SCORE_PER_LEVEL = 50;
};

} // namespace ECS
} // namespace Lite2D
