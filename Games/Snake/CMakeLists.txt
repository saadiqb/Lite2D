# Snake Game CMakeLists.txt

# Snake Game executable
add_executable(snake_game 
    main.cpp
    
    # Game-specific components
    Components/SnakeSegment.cpp
    Components/SnakeHead.cpp
    Components/Food.cpp
    Components/Wall.cpp
    Components/GameState.cpp
    
    # Game-specific systems
    Systems/SnakeMovementSystem.cpp
    Systems/CollisionSystem.cpp
    Systems/InputSystem.cpp
    Systems/GameLogicSystem.cpp
    
    # Game logic
    Game/SnakeGame.cpp
)

target_link_libraries(snake_game PRIVATE Lite2D)

if(WIN32)
    set_target_properties(snake_game PROPERTIES WIN32_EXECUTABLE YES)
endif()