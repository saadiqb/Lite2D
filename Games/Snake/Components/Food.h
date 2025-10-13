#pragma once

#include "../../ECS/Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Food component
 * Represents food items that the snake can eat
 */
class Food : public Component {
public:
    int points; // Points awarded when eaten
    bool isActive; // Whether this food is currently available
    
    Food(int pointsValue = 10, bool active = true) 
        : points(pointsValue), isActive(active) {}
    
    // Component interface
    Component* Clone() const override {
        return new Food(points, isActive);
    }
    
    const char* GetTypeName() const override {
        return "Food";
    }
    
    static const char* GetTypeNameStatic() {
        return "Food";
    }
};

} // namespace ECS
} // namespace Lite2D
