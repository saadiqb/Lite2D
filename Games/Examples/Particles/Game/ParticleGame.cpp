#include "ParticleGame.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace Lite2D {
namespace ECS {

ParticleGame::ParticleGame() 
    : mWindow(nullptr), mRenderer(nullptr), mWindowWidth(1920), mWindowHeight(1080),
      mIsRunning(false), mLastFrameTime(0.0f), mFPS(0.0f), mFPSTimer(0.0f), mFrameCount(0) {
}

ParticleGame::~ParticleGame() {
    Shutdown();
}

bool ParticleGame::Initialize(int windowWidth, int windowHeight) {
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    
    if (!InitializeSDL()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }
    
    if (!InitializeECS()) {
        std::cerr << "Failed to initialize ECS" << std::endl;
        return false;
    }
    
    ConfigureSystems();
    InitializeParticles();
    
    std::cout << "Particle Game initialized successfully!" << std::endl;
    PrintInstructions();
    
    return true;
}

bool ParticleGame::InitializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (!SDL_CreateWindowAndRenderer("Lite2D - Colliding Particles Demo", 
                                   mWindowWidth, mWindowHeight, 0, &mWindow, &mRenderer)) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Enable VSync
    if (SDL_SetRenderVSync(mRenderer, true) != 0) {
        std::cerr << "VSync could not be enabled! SDL_Error: " << SDL_GetError() << std::endl;
        // Continue without VSync
    }
    
    // Note: TTF and Image initialization are handled by the Lite2D library
    
    return true;
}

bool ParticleGame::InitializeECS() {
    // Create ECS managers
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
    
    // Register systems
    mMovementSystem = mSystemManager->RegisterSystem<MovementSystem>();
    mRenderSystem = mSystemManager->RegisterSystem<RenderSystem>(mRenderer);
    mCollisionSystem = mSystemManager->RegisterSystem<CollisionSystem>();
    mParticleSystem = mSystemManager->RegisterSystem<ParticleSystem>();
    
    // Set system signatures
    mSystemManager->SetSystemSignature<MovementSystem>(
        mEntityManager->GetComponentSignature<Position, Velocity>()
    );
    mSystemManager->SetSystemSignature<RenderSystem>(
        mEntityManager->GetComponentSignature<Position, Renderable>()
    );
    mSystemManager->SetSystemSignature<CollisionSystem>(
        mEntityManager->GetComponentSignature<Position, Velocity, Particle>()
    );
    mSystemManager->SetSystemSignature<ParticleSystem>(
        mEntityManager->GetComponentSignature<Position, Particle>()
    );
    
    // Initialize systems
    mSystemManager->InitializeAllSystems(*mEntityManager);
    
    return true;
}

void ParticleGame::ConfigureSystems() {
    // Configure movement system
    mMovementSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
    mMovementSystem->EnableBoundaryClamping(false); // Let collision system handle boundaries
    
    // Configure render system
    mRenderSystem->SetClearColor(10, 10, 20, 255); // Dark blue background
    mRenderSystem->SetRenderOrder(true); // Lower layers first
    
    // Configure collision system
    mCollisionSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
    mCollisionSystem->SetElasticity(0.9f); // High elasticity for bouncy particles
    mCollisionSystem->SetFriction(0.02f);  // Low friction
    
    // Configure particle system
    mParticleSystem->SetSpawnArea(50, 50, mWindowWidth - 50, mWindowHeight - 50);
    mParticleSystem->SetVelocityRange(50.0f, 300.0f);
    mParticleSystem->SetSizeRange(8.0f, 20.0f);
    mParticleSystem->SetMassRange(0.8f, 2.5f);
    mParticleSystem->SetLifetimeRange(15.0f, 45.0f);
    mParticleSystem->SetColorRange(100, 255, 100, 255, 100, 255, 180, 255);
    mParticleSystem->SetMaxParticles(500);
}

void ParticleGame::InitializeParticles() {
    // Spawn initial particles
    mParticleSystem->SpawnParticleBurst(*mEntityManager, 50, 
                                       mWindowWidth / 2.0f, mWindowHeight / 2.0f);
    
    // Enable auto-spawning for continuous animation
    mParticleSystem->EnableAutoSpawn(true, 0.5f);
}

void ParticleGame::Run() {
    if (!mIsRunning) {
        mIsRunning = true;
        mLastFrameTime = SDL_GetTicks() / 1000.0f;
        
        std::cout << "Starting Particle Animation..." << std::endl;
    }
    
    while (mIsRunning) {
        HandleEvents();
        
        float currentTime = SDL_GetTicks() / 1000.0f;
        float deltaTime = currentTime - mLastFrameTime;
        mLastFrameTime = currentTime;
        
        Update(deltaTime);
        Render();
        
        // Cap frame rate to ~60 FPS
        SDL_Delay(16);
    }
}

void ParticleGame::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                mIsRunning = false;
                break;
                
            case SDL_EVENT_KEY_DOWN:
                HandleKeyboardInput();
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    HandleMouseInput(event.button.x, event.button.y, true);
                }
                break;
                
            case SDL_EVENT_WINDOW_RESIZED:
                mWindowWidth = event.window.data1;
                mWindowHeight = event.window.data2;
                // Update system boundaries
                mMovementSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
                mCollisionSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
                mParticleSystem->SetSpawnArea(50, 50, mWindowWidth - 50, mWindowHeight - 50);
                break;
        }
    }
}

void ParticleGame::HandleKeyboardInput() {
    const bool* keystate = SDL_GetKeyboardState(nullptr);
    
    if (keystate[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
    
    if (keystate[SDL_SCANCODE_SPACE]) {
        // Spawn particle burst at center
        mParticleSystem->SpawnParticleBurst(*mEntityManager, 20, 
                                           mWindowWidth / 2.0f, mWindowHeight / 2.0f);
    }
    
    if (keystate[SDL_SCANCODE_R]) {
        // Reset/clear all particles
        mParticleSystem->ClearAllParticles(*mEntityManager);
        mCollisionSystem->ResetCollisionCount();
        mParticleSystem->ResetStatistics();
    }
    
    if (keystate[SDL_SCANCODE_1]) {
        SetDemoMode(BASIC_DEMO);
    }
    
    if (keystate[SDL_SCANCODE_2]) {
        SetDemoMode(STRESS_TEST);
    }
    
    if (keystate[SDL_SCANCODE_3]) {
        SetDemoMode(BURST_DEMO);
    }
    
    if (keystate[SDL_SCANCODE_4]) {
        SetDemoMode(RAINBOW_DEMO);
    }
    
    if (keystate[SDL_SCANCODE_S]) {
        PrintStatistics();
    }
}

void ParticleGame::HandleMouseInput(int mouseX, int mouseY, bool leftClick) {
    if (leftClick) {
        // Spawn particles at mouse position
        mParticleSystem->SpawnParticleBurst(*mEntityManager, 10, mouseX, mouseY);
    }
}

void ParticleGame::Update(float deltaTime) {
    // Update systems
    mSystemManager->UpdateSystems(*mEntityManager, deltaTime);
    
    // Update FPS calculation
    mFrameCount++;
    mFPSTimer += deltaTime;
    if (mFPSTimer >= 1.0f) {
        mFPS = mFrameCount / mFPSTimer;
        mFrameCount = 0;
        mFPSTimer = 0.0f;
    }
    
    // Update demo mode
    UpdateDemoMode(deltaTime);
}

void ParticleGame::Render() {
    // Clear the screen with background color
    SDL_SetRenderDrawColor(mRenderer, 10, 10, 20, 255); // Dark blue background
    SDL_RenderClear(mRenderer);
    
    // Render all particles
    auto entities = mEntityManager->GetEntitiesWith<Position, Renderable, Particle>();
    
    for (Entity entity : entities) {
        Position* pos = mEntityManager->GetComponent<Position>(entity);
        Particle* particle = mEntityManager->GetComponent<Particle>(entity);
        
        if (pos && particle && particle->isActive) {
            // Set particle color
            SDL_SetRenderDrawColor(mRenderer, particle->r, particle->g, particle->b, particle->a);
            
            // Render particle as a solid filled circle
            float radius = particle->radius;
            if (radius <= 0) continue; // Skip invalid radius
            
            float centerX = pos->x;
            float centerY = pos->y;
            
            // Draw a solid filled circle using a simple but effective method
            // We'll draw the circle by filling it row by row
            int radiusInt = static_cast<int>(radius);
            
            for (int y = -radiusInt; y <= radiusInt; y++) {
                int x = static_cast<int>(std::sqrt(radius * radius - y * y));
                if (x > 0) {
                    // Draw a horizontal line across the circle at this y position
                    SDL_FRect lineRect = {
                        centerX - x, centerY + y,
                        x * 2.0f, 1.0f
                    };
                    SDL_RenderFillRect(mRenderer, &lineRect);
                }
            }
        }
    }
    
    // Present the frame
    SDL_RenderPresent(mRenderer);
}

void ParticleGame::SetDemoMode(DemoMode mode) {
    mCurrentMode = mode;
    
    switch (mode) {
        case BASIC_DEMO:
            std::cout << "Demo Mode: Basic Particle Animation" << std::endl;
            mParticleSystem->SetMaxParticles(100);
            mParticleSystem->EnableAutoSpawn(true, 1.0f);
            break;
            
        case STRESS_TEST:
            std::cout << "Demo Mode: Stress Test (High Particle Count)" << std::endl;
            mParticleSystem->SetMaxParticles(1000);
            mParticleSystem->EnableAutoSpawn(true, 0.1f);
            break;
            
        case BURST_DEMO:
            std::cout << "Demo Mode: Burst Animation" << std::endl;
            mParticleSystem->SetMaxParticles(200);
            mParticleSystem->EnableAutoSpawn(false, 0.0f);
            mParticleSystem->SpawnParticleBurst(*mEntityManager, 50, 
                                               mWindowWidth / 2.0f, mWindowHeight / 2.0f);
            break;
            
        case RAINBOW_DEMO:
            std::cout << "Demo Mode: Rainbow Colors" << std::endl;
            mParticleSystem->SetMaxParticles(150);
            mParticleSystem->SetColorRange(50, 255, 50, 255, 50, 255, 200, 255);
            break;
    }
}

void ParticleGame::UpdateDemoMode(float deltaTime) {
    // Demo-specific logic can be added here
    static float demoTimer = 0.0f;
    demoTimer += deltaTime;
    
    if (mCurrentMode == BURST_DEMO && demoTimer >= 3.0f) {
        // Spawn new bursts periodically
        mParticleSystem->SpawnParticleBurst(*mEntityManager, 30, 
                                           mWindowWidth / 2.0f, mWindowHeight / 2.0f);
        demoTimer = 0.0f;
    }
}

void ParticleGame::PrintInstructions() {
    std::cout << "\n=== Particle Animation Controls ===" << std::endl;
    std::cout << "Mouse Left Click: Spawn particles at cursor" << std::endl;
    std::cout << "SPACE: Spawn particle burst at center" << std::endl;
    std::cout << "R: Reset/Clear all particles" << std::endl;
    std::cout << "1: Basic Demo Mode" << std::endl;
    std::cout << "2: Stress Test Mode" << std::endl;
    std::cout << "3: Burst Demo Mode" << std::endl;
    std::cout << "4: Rainbow Demo Mode" << std::endl;
    std::cout << "S: Print Statistics" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "=====================================\n" << std::endl;
}

void ParticleGame::PrintStatistics() {
    std::cout << "\n=== Particle System Statistics ===" << std::endl;
    std::cout << "FPS: " << std::fixed << std::setprecision(1) << mFPS << std::endl;
    std::cout << "Active Particles: " << mParticleSystem->GetActiveParticleCount() << std::endl;
    std::cout << "Total Spawned: " << mParticleSystem->GetTotalParticlesSpawned() << std::endl;
    std::cout << "Collisions This Frame: " << mCollisionSystem->GetCollisionCount() << std::endl;
    std::cout << "Window Size: " << mWindowWidth << "x" << mWindowHeight << std::endl;
    std::cout << "===================================\n" << std::endl;
}

void ParticleGame::Shutdown() {
    CleanupECS();
    CleanupSDL();
}

void ParticleGame::CleanupSDL() {
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    
    // Cleanup SDL
    SDL_Quit();
}

void ParticleGame::CleanupECS() {
    if (mSystemManager) {
        mSystemManager->ShutdownAllSystems(*mEntityManager);
    }
    
    mSystemManager.reset();
    mEntityManager.reset();
}

} // namespace ECS
} // namespace Lite2D
