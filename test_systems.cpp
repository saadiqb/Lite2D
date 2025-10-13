#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
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
    
    // Create test entities
    Entity player = entityManager.CreateEntity();
    entityManager.AddComponent(player, Position(100.0f, 100.0f));
    entityManager.AddComponent(player, Velocity(50.0f, 30.0f));
    entityManager.AddComponent(player, Renderable(true, 1));
    
    Entity enemy = entityManager.CreateEntity();
    entityManager.AddComponent(enemy, Position(300.0f, 200.0f));
    entityManager.AddComponent(enemy, Velocity(-25.0f, 15.0f));
    entityManager.AddComponent(enemy, Renderable(true, 0));
    
    Entity item = entityManager.CreateEntity();
    entityManager.AddComponent(item, Position(400.0f, 300.0f));
    entityManager.AddComponent(item, Renderable(true, 2));
    // Note: item has no velocity, so it won't move
    
    std::cout << "Created test entities:" << std::endl;
    std::cout << "- Player (moving): " << player << std::endl;
    std::cout << "- Enemy (moving): " << enemy << std::endl;
    std::cout << "- Item (static): " << item << std::endl;
    
    // Configure systems
    movementSystem->SetBoundaries(0, 0, 800, 600);
    movementSystem->EnableBoundaryClamping(true);
    movementSystem->SetMaxSpeed(200.0f);
    
    renderSystem->SetClearColor(20, 20, 40, 255); // Dark blue background
    renderSystem->SetRenderOrder(true); // Lower layers first
    renderSystem->EnableDebugInfo(true);
    
    // Print system info
    systemManager.PrintSystemInfo();
    
    // Main loop
    bool running = true;
    SDL_Event event;
    
    std::cout << "Starting main loop... (Press ESC or close window to exit)" << std::endl;
    
    while (running) {
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
        
        // Update systems (60 FPS = ~16.67ms per frame)
        float deltaTime = 16.67f / 1000.0f; // Convert to seconds
        systemManager.UpdateSystems(entityManager, deltaTime);
        
        // Small delay to prevent 100% CPU usage
        SDL_Delay(16);
    }
    
    // Cleanup
    systemManager.ShutdownAllSystems(entityManager);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    std::cout << "ECS Systems test completed!" << std::endl;
    
    return 0;
}
