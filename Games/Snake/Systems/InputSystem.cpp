#include "InputSystem.h"
#include <iostream>

namespace Lite2D {
namespace ECS {

void InputSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled) return;
    
    // Input processing is handled in ProcessEvent, not in Update
    // This system mainly manages input state
}

void InputSystem::Initialize(EntityManager& entityManager) {
    std::cout << "InputSystem initialized" << std::endl;
}

void InputSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "InputSystem shutdown" << std::endl;
}

void InputSystem::ProcessEvent(SDL_Event& event, EntityManager& entityManager) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        HandleSnakeMovement(event, entityManager);
        HandleGameControls(event, entityManager);
    }
}

void InputSystem::HandleSnakeMovement(SDL_Event& event, EntityManager& entityManager) {
    if (mSnakeHeadEntity == INVALID_ENTITY) return;
    
    SnakeHead* snakeHead = entityManager.GetComponent<SnakeHead>(mSnakeHeadEntity);
    if (!snakeHead) return;
    
    // Get game state to check if we're in playing mode
    GameState* gameState = nullptr;
    if (mGameStateEntity != INVALID_ENTITY) {
        gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    }
    
    // Only process movement input when playing
    if (gameState && gameState->currentState != GameState::PLAYING) {
        return;
    }
    
    SnakeHead::Direction newDirection = GetDirectionFromKey(event.key.key);
    if (newDirection != snakeHead->currentDirection) {
        snakeHead->SetDirection(newDirection);
    }
}

void InputSystem::HandleGameControls(SDL_Event& event, EntityManager& entityManager) {
    if (mGameStateEntity == INVALID_ENTITY) return;
    
    GameState* gameState = entityManager.GetComponent<GameState>(mGameStateEntity);
    if (!gameState) return;
    
    switch (event.key.key) {
        case SDLK_SPACE:
            // Start game from menu or pause/unpause
            if (gameState->currentState == GameState::MENU) {
                gameState->currentState = GameState::PLAYING;
            } else if (gameState->currentState == GameState::PLAYING) {
                gameState->currentState = GameState::PAUSED;
            } else if (gameState->currentState == GameState::PAUSED) {
                gameState->currentState = GameState::PLAYING;
            }
            break;
            
        case SDLK_R:
            // Restart game from game over
            if (gameState->currentState == GameState::GAME_OVER) {
                gameState->ResetGame();
            }
            break;
            
        case SDLK_Q:
            // Quit to menu
            if (gameState->currentState == GameState::GAME_OVER || 
                gameState->currentState == GameState::PAUSED) {
                gameState->currentState = GameState::MENU;
            }
            break;
            
        case SDLK_ESCAPE:
            // Exit application
            // This would typically be handled by the main loop
            break;
    }
}

SnakeHead::Direction InputSystem::GetDirectionFromKey(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
        case SDLK_W:
            return SnakeHead::UP;
        case SDLK_DOWN:
        case SDLK_S:
            return SnakeHead::DOWN;
        case SDLK_LEFT:
        case SDLK_A:
            return SnakeHead::LEFT;
        case SDLK_RIGHT:
        case SDLK_D:
            return SnakeHead::RIGHT;
        default:
            return SnakeHead::RIGHT; // Default direction
    }
}

} // namespace ECS
} // namespace Lite2D
