#pragma once

#include "../Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Velocity component for 2D movement
 */
class Velocity : public Component {
public:
    float x, y;
    
    Velocity(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    // Component interface
    Component* Clone() const override {
        return new Velocity(x, y);
    }
    
    const char* GetTypeName() const override {
        return "Velocity";
    }
    
    static const char* GetTypeNameStatic() {
        return "Velocity";
    }
};

} // namespace ECS
} // namespace Lite2D
