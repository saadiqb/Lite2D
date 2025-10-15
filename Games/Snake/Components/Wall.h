#pragma once

#include "ECS/Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Wall component
 * Represents boundary walls and obstacles
 */
class Wall : public Component {
public:
    enum WallType {
        BOUNDARY = 0,  // Game boundary wall
        OBSTACLE = 1   // Internal obstacle
    };
    
    WallType wallType;
    
    Wall(WallType type = BOUNDARY) : wallType(type) {}
    
    // Component interface
    Component* Clone() const override {
        return new Wall(wallType);
    }
    
    const char* GetTypeName() const override {
        return "Wall";
    }
    
    static const char* GetTypeNameStatic() {
        return "Wall";
    }
};

} // namespace ECS
} // namespace Lite2D
