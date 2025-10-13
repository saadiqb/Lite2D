#include <iostream>
#include "Game/SnakeGame.h"

using namespace Lite2D::ECS;

int main() {
    std::cout << "=== Lite2D Snake Game ===" << std::endl;
    std::cout << "A demonstration of the Lite2D ECS Game Engine" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Create and initialize game
    SnakeGame game;
    
    if (!game.Initialize(800, 600)) {
        std::cerr << "Failed to initialize Snake Game!" << std::endl;
        return -1;
    }
    
    // Run the game
    game.Run();
    
    // Game will shutdown automatically when done
    std::cout << "Thanks for playing!" << std::endl;
    
    return 0;
}
