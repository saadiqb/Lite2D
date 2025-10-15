#pragma once

#include "../Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Position component for 2D coordinates
 */
class Position : public Component {
public:
    float x, y;
    
    Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    // Component interface
    Component* Clone() const override {
        return new Position(x, y);
    }
    
    const char* GetTypeName() const override {
        return "Position";
    }
    
    static const char* GetTypeNameStatic() {
        return "Position";
    }
};

} // namespace ECS
} // namespace Lite2D
