#pragma once

#include "ECS/Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Snake Head component
 * Represents the snake's head with movement direction
 */
class SnakeHead : public Component {
public:
    enum Direction {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };
    
    Direction currentDirection;
    Direction nextDirection;
    float moveTimer;
    float moveInterval; // Time between moves in seconds
    int segmentsToAdd; // Number of segments to add when eating food
    
    SnakeHead(Direction direction = RIGHT, float interval = 0.2f) 
        : currentDirection(direction), nextDirection(direction), 
          moveTimer(0.0f), moveInterval(interval), segmentsToAdd(0) {}
    
    // Component interface
    Component* Clone() const override {
        return new SnakeHead(currentDirection, moveInterval);
    }
    
    const char* GetTypeName() const override {
        return "SnakeHead";
    }
    
    static const char* GetTypeNameStatic() {
        return "SnakeHead";
    }
    
    // Helper functions
    void SetDirection(Direction direction) {
        // Prevent reversing into self
        if ((currentDirection == UP && direction == DOWN) ||
            (currentDirection == DOWN && direction == UP) ||
            (currentDirection == LEFT && direction == RIGHT) ||
            (currentDirection == RIGHT && direction == LEFT)) {
            return;
        }
        nextDirection = direction;
    }
    
    void GetDirectionVector(float& x, float& y) const {
        x = y = 0.0f;
        switch (currentDirection) {
            case UP:    y = -1.0f; break;
            case DOWN:  y = 1.0f; break;
            case LEFT:  x = -1.0f; break;
            case RIGHT: x = 1.0f; break;
        }
    }
};

} // namespace ECS
} // namespace Lite2D
