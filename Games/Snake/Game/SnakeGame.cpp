#include "SnakeGame.h"
#include <iostream>
#include <memory>

namespace Lite2D {
namespace ECS {

SnakeGame::SnakeGame() 
    : mWindow(nullptr), mRenderer(nullptr), mWindowWidth(800), mWindowHeight(600),
      mIsRunning(false), mLastFrameTime(0.0f), mGameStateEntity(INVALID_ENTITY),
      mSnakeHeadEntity(INVALID_ENTITY) {
}

SnakeGame::~SnakeGame() {
    Shutdown();
}

bool SnakeGame::Initialize(int windowWidth, int windowHeight) {
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    
    // Initialize SDL
    if (!InitializeSDL()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }
    
    // Initialize ECS
    if (!InitializeECS()) {
        std::cerr << "Failed to initialize ECS" << std::endl;
        return false;
    }
    
    // Initialize game
    InitializeGame();
    
    std::cout << "Snake Game initialized successfully!" << std::endl;
    return true;
}

bool SnakeGame::InitializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create window
    mWindow = SDL_CreateWindow("Snake Game - Lite2D ECS", mWindowWidth, mWindowHeight, 0);
    if (!mWindow) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    mRenderer = SDL_CreateRenderer(mWindow, nullptr);
    if (!mRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Enable VSync
    if (!SDL_SetRenderVSync(mRenderer, true)) {
        std::cerr << "VSync could not be enabled! SDL_Error: " << SDL_GetError() << std::endl;
        // Continue without VSync
    }
    
    return true;
}

bool SnakeGame::InitializeECS() {
    // Create ECS managers
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
    
    // Register systems
    mMovementSystem = mSystemManager->RegisterSystem<MovementSystem>();
    mRenderSystem = mSystemManager->RegisterSystem<RenderSystem>(mRenderer);
    mInputSystem = mSystemManager->RegisterSystem<InputSystem>();
    mSnakeMovementSystem = mSystemManager->RegisterSystem<SnakeMovementSystem>();
    mCollisionSystem = mSystemManager->RegisterSystem<CollisionSystem>();
    mGameLogicSystem = mSystemManager->RegisterSystem<GameLogicSystem>();
    
    // Set system signatures
    mSystemManager->SetSystemSignature<MovementSystem>(
        mEntityManager->GetComponentSignature<Position, Velocity>()
    );
    mSystemManager->SetSystemSignature<RenderSystem>(
        mEntityManager->GetComponentSignature<Position, Renderable>()
    );
    mSystemManager->SetSystemSignature<SnakeMovementSystem>(
        mEntityManager->GetComponentSignature<Position, SnakeHead>()
    );
    mSystemManager->SetSystemSignature<CollisionSystem>(
        mEntityManager->GetComponentSignature<Position>()
    );
    mSystemManager->SetSystemSignature<GameLogicSystem>(
        mEntityManager->GetComponentSignature<GameState>()
    );
    
    // Initialize systems
    mSystemManager->InitializeAllSystems(*mEntityManager);
    
    // Configure systems
    mMovementSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
    mMovementSystem->EnableBoundaryClamping(false); // Let collision system handle boundaries
    
    mRenderSystem->SetClearColor(20, 20, 40, 255); // Dark blue background
    mRenderSystem->SetRenderOrder(true); // Lower layers first
    
    mCollisionSystem->SetBoundaries(0, 0, mWindowWidth, mWindowHeight);
    
    return true;
}

void SnakeGame::InitializeGame() {
    // Create game state entity
    mGameStateEntity = mEntityManager->CreateEntity();
    mEntityManager->AddComponent(mGameStateEntity, GameState());
    
    // Set up system references
    mInputSystem->SetGameStateEntity(mGameStateEntity);
    mSnakeMovementSystem->SetGameStateEntity(mGameStateEntity);
    mCollisionSystem->SetGameStateEntity(mGameStateEntity);
    mGameLogicSystem->SetGameStateEntity(mGameStateEntity);
    
    // Start new game
    mGameLogicSystem->StartNewGame(*mEntityManager);
    
    // Get snake head entity after game initialization
    auto snakeEntities = mEntityManager->GetEntitiesWith<SnakeHead>();
    if (!snakeEntities.empty()) {
        mSnakeHeadEntity = snakeEntities[0];
        mInputSystem->SetSnakeHeadEntity(mSnakeHeadEntity);
        mSnakeMovementSystem->SetSnakeHeadEntity(mSnakeHeadEntity);
        mCollisionSystem->SetSnakeHeadEntity(mSnakeHeadEntity);
        mGameLogicSystem->SetSnakeHeadEntity(mSnakeHeadEntity);
    }
    
}

void SnakeGame::Run() {
    if (!mIsRunning) {
        mIsRunning = true;
        mLastFrameTime = SDL_GetTicks() / 1000.0f;
        
        std::cout << "Starting Snake Game..." << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  Arrow Keys or WASD - Move snake" << std::endl;
        std::cout << "  Space - Start/Pause game" << std::endl;
        std::cout << "  R - Restart (from game over)" << std::endl;
        std::cout << "  Q - Quit to menu" << std::endl;
        std::cout << "  ESC - Exit game" << std::endl;
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

void SnakeGame::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            mIsRunning = false;
        }
        
        // Process input through input system
        mInputSystem->ProcessEvent(event, *mEntityManager);
    }
}

void SnakeGame::Update(float deltaTime) {
    // Update all systems
    mSystemManager->UpdateSystems(*mEntityManager, deltaTime);
}

void SnakeGame::Render() {
    // Render all entities through render system (handles clearing and presenting internally)
    mRenderSystem->Update(*mEntityManager, 0.0f); // deltaTime not needed for rendering
    // Note: RenderSystem handles both clearing and presenting the frame
}

void SnakeGame::Shutdown() {
    CleanupECS();
    CleanupSDL();
    std::cout << "Snake Game shutdown complete" << std::endl;
}

void SnakeGame::CleanupSDL() {
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    
    SDL_Quit();
}

void SnakeGame::CleanupECS() {
    if (mSystemManager) {
        mSystemManager->ShutdownAllSystems(*mEntityManager);
        mSystemManager.reset();
    }
    
    if (mEntityManager) {
        mEntityManager->Clear();
        mEntityManager.reset();
    }
}

} // namespace ECS
} // namespace Lite2D
