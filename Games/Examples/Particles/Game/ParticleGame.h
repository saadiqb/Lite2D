#pragma once

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "Rendering/RenderSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/ParticleSystem.h"
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Particle Game Class
 * Main controller for the colliding particles animation
 */
class ParticleGame {
public:
    ParticleGame();
    ~ParticleGame();
    
    // Game lifecycle
    bool Initialize(int windowWidth = 1920, int windowHeight = 1080);
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
    
    // Game configuration
    int mWindowWidth;
    int mWindowHeight;
    bool mIsRunning;
    
    // Timing
    float mLastFrameTime;
    float mFPS;
    float mFPSTimer;
    int mFrameCount;
    
    // System references
    std::shared_ptr<MovementSystem> mMovementSystem;
    std::shared_ptr<RenderSystem> mRenderSystem;
    std::shared_ptr<CollisionSystem> mCollisionSystem;
    std::shared_ptr<ParticleSystem> mParticleSystem;
    
    // Game loop
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    
    // Initialization helpers
    bool InitializeSDL();
    bool InitializeECS();
    void InitializeParticles();
    
    // Cleanup
    void CleanupSDL();
    void CleanupECS();
    
    // Input handling
    void HandleKeyboardInput();
    void HandleMouseInput(int mouseX, int mouseY, bool leftClick);
    
    // Game configuration
    void ConfigureSystems();
    void PrintInstructions();
    void PrintStatistics();
    
    // Demo modes
    enum DemoMode {
        BASIC_DEMO,
        STRESS_TEST,
        BURST_DEMO,
        RAINBOW_DEMO
    };
    DemoMode mCurrentMode = BASIC_DEMO;
    void SetDemoMode(DemoMode mode);
    void UpdateDemoMode(float deltaTime);
};

} // namespace ECS
} // namespace Lite2D
