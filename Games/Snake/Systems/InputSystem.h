#pragma once

#include "../../ECS/System.h"
#include "../../ECS/EntityManager.h"
#include "../Components/SnakeHead.h"
#include "../Components/GameState.h"
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Input System for Snake Game
 * Processes keyboard input and updates game accordingly
 */
class InputSystem : public System {
public:
    InputSystem() = default;
    ~InputSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "InputSystem"; }
    
    // Input handling
    void ProcessEvent(SDL_Event& event, EntityManager& entityManager);
    
    // Game controls
    void SetSnakeHeadEntity(Entity snakeHead) { mSnakeHeadEntity = snakeHead; }
    void SetGameStateEntity(Entity gameStateEntity) { mGameStateEntity = gameStateEntity; }

private:
    Entity mSnakeHeadEntity = INVALID_ENTITY;
    Entity mGameStateEntity = INVALID_ENTITY;
    
    // Input processing helpers
    void HandleSnakeMovement(SDL_Event& event, EntityManager& entityManager);
    void HandleGameControls(SDL_Event& event, EntityManager& entityManager);
    SnakeHead::Direction GetDirectionFromKey(SDL_Keycode key);
};

} // namespace ECS
} // namespace Lite2D
