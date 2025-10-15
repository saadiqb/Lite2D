#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <chrono>
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"

using namespace Lite2D::ECS;

int main() {
    std::cout << "Testing Lite2D ECS Systems..." << std::endl;
    
    // Initialize SDL for rendering
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("ECS Systems Test", 800, 600, 0);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    // Create ECS managers
    EntityManager entityManager;
    SystemManager systemManager;
    
    // Register systems
    auto movementSystem = systemManager.RegisterSystem<MovementSystem>();
    auto renderSystem = systemManager.RegisterSystem<RenderSystem>(renderer);
    
    // Set system signatures (which components each system requires)
    systemManager.SetSystemSignature<MovementSystem>(
        entityManager.GetComponentSignature<Position, Velocity>()
    );
    systemManager.SetSystemSignature<RenderSystem>(
        entityManager.GetComponentSignature<Position, Renderable>()
    );
    
    // Initialize systems
    systemManager.InitializeAllSystems(entityManager);
    
    // Create test entities for bouncing balls animation
    Entity ball1 = entityManager.CreateEntity();
    entityManager.AddComponent(ball1, Position(150.0f, 200.0f));
    entityManager.AddComponent(ball1, Velocity(80.0f, 60.0f)); // Diagonal movement
    entityManager.AddComponent(ball1, Renderable(true, 1));
    
    Entity ball2 = entityManager.CreateEntity();
    entityManager.AddComponent(ball2, Position(400.0f, 300.0f));
    entityManager.AddComponent(ball2, Velocity(-60.0f, 80.0f)); // Different diagonal
    entityManager.AddComponent(ball2, Renderable(true, 0));
    
    Entity ball3 = entityManager.CreateEntity();
    entityManager.AddComponent(ball3, Position(650.0f, 150.0f));
    entityManager.AddComponent(ball3, Velocity(-40.0f, -50.0f)); // Third direction
    entityManager.AddComponent(ball3, Renderable(true, 2));
    
    std::cout << "Created test entities:" << std::endl;
    std::cout << "- Ball 1 (bouncing): " << ball1 << std::endl;
    std::cout << "- Ball 2 (bouncing): " << ball2 << std::endl;
    std::cout << "- Ball 3 (bouncing): " << ball3 << std::endl;
    
    // Configure systems
    movementSystem->SetBoundaries(0, 0, 800, 600);
    movementSystem->EnableBoundaryClamping(true);
    movementSystem->SetMaxSpeed(200.0f);
    
    renderSystem->SetClearColor(20, 20, 40, 255); // Dark blue background
    renderSystem->SetRenderOrder(true); // Lower layers first
    renderSystem->EnableDebugInfo(false);
    
    // Print system info
    systemManager.PrintSystemInfo();
    
    // Bouncing balls animation - no custom logic needed!
    // MovementSystem will handle boundary bouncing automatically
    
    // Main loop
    bool running = true;
    SDL_Event event;
    
    std::cout << "Starting main loop... (Press ESC or close window to exit)" << std::endl;
    
    // Frame timing variables
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto frameStartTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float totalFrameTime = 0.0f;
    
    while (running) {
        frameStartTime = std::chrono::high_resolution_clock::now();
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
        
        // Per-frame entity tracking for debugging (every 30 frames)
        if (frameCount % 30 == 0) {
            std::cout << "[ENTITY TRACKING] Frame " << frameCount << ":" << std::endl;
            
            Position* ball1Pos = entityManager.GetComponent<Position>(ball1);
            Velocity* ball1Vel = entityManager.GetComponent<Velocity>(ball1);
            Position* ball2Pos = entityManager.GetComponent<Position>(ball2);
            Velocity* ball2Vel = entityManager.GetComponent<Velocity>(ball2);
            Position* ball3Pos = entityManager.GetComponent<Position>(ball3);
            Velocity* ball3Vel = entityManager.GetComponent<Velocity>(ball3);
            
            if (ball1Pos && ball1Vel) {
                std::cout << "  Ball1: Pos(" << ball1Pos->x << ", " << ball1Pos->y << ") Vel(" << ball1Vel->x << ", " << ball1Vel->y << ")" << std::endl;
            }
            if (ball2Pos && ball2Vel) {
                std::cout << "  Ball2: Pos(" << ball2Pos->x << ", " << ball2Pos->y << ") Vel(" << ball2Vel->x << ", " << ball2Vel->y << ")" << std::endl;
            }
            if (ball3Pos && ball3Vel) {
                std::cout << "  Ball3: Pos(" << ball3Pos->x << ", " << ball3Pos->y << ") Vel(" << ball3Vel->x << ", " << ball3Vel->y << ")" << std::endl;
            }
        }
        
        // Calculate actual frame time
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float actualFrameTime = std::chrono::duration<float, std::milli>(currentFrameTime - lastFrameTime).count();
        float deltaTime = actualFrameTime / 1000.0f; // Convert to seconds
        
        // Detailed system timing breakdown
        auto systemUpdateStart = std::chrono::high_resolution_clock::now();
        
        // Time individual system updates
        auto movementStart = std::chrono::high_resolution_clock::now();
        auto movementSystem = systemManager.GetSystem<MovementSystem>();
        if (movementSystem && movementSystem->IsEnabled()) {
            movementSystem->Update(entityManager, deltaTime);
        }
        auto movementEnd = std::chrono::high_resolution_clock::now();
        float movementTime = std::chrono::duration<float, std::milli>(movementEnd - movementStart).count();
        
        auto renderStart = std::chrono::high_resolution_clock::now();
        auto renderSystem = systemManager.GetSystem<RenderSystem>();
        if (renderSystem && renderSystem->IsEnabled()) {
            renderSystem->Update(entityManager, deltaTime);
        }
        auto renderEnd = std::chrono::high_resolution_clock::now();
        float renderTime = std::chrono::duration<float, std::milli>(renderEnd - renderStart).count();
        
        auto systemUpdateEnd = std::chrono::high_resolution_clock::now();
        float systemUpdateTime = std::chrono::duration<float, std::milli>(systemUpdateEnd - systemUpdateStart).count();
        
        // Calculate total frame time
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        float totalFrameDuration = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();
        
        // Log timing every 60 frames (roughly once per second)
        frameCount++;
        totalFrameTime += actualFrameTime;
        
        // Adaptive frame timing to prevent choppiness
        float targetFrameTime = 16.67f; // 60 FPS target
        
        if (frameCount % 60 == 0) {
            float avgFrameTime = totalFrameTime / 60.0f;
            float expectedFrameTime = 16.67f;
            float fps = 1000.0f / avgFrameTime;
            
            // Get detailed ECS statistics
            size_t totalEntities = entityManager.GetEntityCount();
            auto entitiesWithPosition = entityManager.GetEntitiesWith<Position>();
            auto entitiesWithVelocity = entityManager.GetEntitiesWith<Velocity>();
            auto entitiesWithRenderable = entityManager.GetEntitiesWith<Renderable>();
            auto entitiesWithBoth = entityManager.GetEntitiesWith<Position, Renderable>();
            
            std::cout << "\n=== ECS ENGINE PERFORMANCE DEBUG ===" << std::endl;
            std::cout << "[TIMING] Frame " << frameCount 
                      << " | Expected: " << expectedFrameTime << "ms"
                      << " | Actual: " << actualFrameTime << "ms"
                      << " | Avg: " << avgFrameTime << "ms"
                      << " | FPS: " << fps
                      << " | DeltaTime: " << deltaTime << "s" << std::endl;
            
            std::cout << "[SYSTEMS] Movement: " << movementTime << "ms"
                      << " | Render: " << renderTime << "ms"
                      << " | Total Systems: " << systemUpdateTime << "ms" << std::endl;
            
            std::cout << "[ECS STATS] Total Entities: " << totalEntities
                      << " | With Position: " << entitiesWithPosition.size()
                      << " | With Velocity: " << entitiesWithVelocity.size()
                      << " | With Renderable: " << entitiesWithRenderable.size()
                      << " | Being Rendered: " << entitiesWithBoth.size() << std::endl;
            
            std::cout << "[FRAME BUDGET] Total Frame: " << totalFrameDuration << "ms"
                      << " | Remaining: " << (targetFrameTime - totalFrameDuration) << "ms"
                      << " | Budget Usage: " << (totalFrameDuration / targetFrameTime * 100.0f) << "%" << std::endl;
            
            // Performance warnings
            if (totalFrameDuration > targetFrameTime * 1.1f) {
                std::cout << "[WARNING] Frame time exceeded target by " << (totalFrameDuration - targetFrameTime) << "ms!" << std::endl;
            }
            if (movementTime > 5.0f) {
                std::cout << "[WARNING] MovementSystem is slow: " << movementTime << "ms" << std::endl;
            }
            if (renderTime > 10.0f) {
                std::cout << "[WARNING] RenderSystem is slow: " << renderTime << "ms" << std::endl;
            }
            
            std::cout << "=======================================\n" << std::endl;
            
            totalFrameTime = 0.0f; // Reset for next average
        }
        float remainingTime = targetFrameTime - totalFrameDuration;
        
        if (remainingTime > 0) {
            // Only delay if we have time left
            SDL_Delay(static_cast<Uint32>(remainingTime));
        }
        // If we're already over target time, don't delay (catch up)
        
        lastFrameTime = currentFrameTime;
    }
    
    // Cleanup
    systemManager.ShutdownAllSystems(entityManager);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    std::cout << "ECS Systems test completed!" << std::endl;
    
    return 0;
}
