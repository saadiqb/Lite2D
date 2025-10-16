#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <exception>

#include "Game/ParticleGame.h"

int main(int argc, char* argv[]) {
    try {
        // Create and initialize the particle game
        Lite2D::ECS::ParticleGame game;
        
        // Parse command line arguments for window size
        int windowWidth = 1920;
        int windowHeight = 1080;
        
        if (argc >= 3) {
            windowWidth = std::atoi(argv[1]);
            windowHeight = std::atoi(argv[2]);
        }
        
        std::cout << "Lite2D Colliding Particles Demo" << std::endl;
        std::cout << "Window Size: " << windowWidth << "x" << windowHeight << std::endl;
        std::cout << "Initializing..." << std::endl;
        
        if (!game.Initialize(windowWidth, windowHeight)) {
            std::cerr << "Failed to initialize particle game!" << std::endl;
            return -1;
        }
        
        std::cout << "Starting particle animation..." << std::endl;
        
        // Run the game loop
        game.Run();
        
        std::cout << "Particle animation completed." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
        return -1;
    }
    
    return 0;
}
