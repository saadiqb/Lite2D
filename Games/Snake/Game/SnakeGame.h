#pragma once

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "Rendering/RenderSystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/SnakeMovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/GameLogicSystem.h"
#include "../Components/GameState.h"
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Snake Game Class
 * Main game controller that manages the ECS systems and game loop
 */
class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    
    // Game lifecycle
    bool Initialize(int windowWidth = 800, int windowHeight = 600);
    void Run();
    void Shutdown();
    
    // Game state
    bool IsRunning() const { return mIsRunning; }
    void Stop() { mIsRunning = false; }

private:
    // SDL components
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    
    // ECS components
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
    
    // Game entities
    Entity mGameStateEntity;
    Entity mSnakeHeadEntity;
    
    // Game configuration
    int mWindowWidth;
    int mWindowHeight;
    bool mIsRunning;
    
    // Timing
    float mLastFrameTime;
    
    // System references
    std::shared_ptr<MovementSystem> mMovementSystem;
    std::shared_ptr<RenderSystem> mRenderSystem;
    std::shared_ptr<InputSystem> mInputSystem;
    std::shared_ptr<SnakeMovementSystem> mSnakeMovementSystem;
    std::shared_ptr<CollisionSystem> mCollisionSystem;
    std::shared_ptr<GameLogicSystem> mGameLogicSystem;
    
    // Game loop
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    
    // Initialization helpers
    bool InitializeSDL();
    bool InitializeECS();
    void InitializeGame();
    
    // Cleanup
    void CleanupSDL();
    void CleanupECS();
};

} // namespace ECS
} // namespace Lite2D
