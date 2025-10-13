#pragma once

#include "../Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Renderable component for entities that should be drawn
 */
class Renderable : public Component {
public:
    bool visible;
    int layer; // Rendering layer (higher = on top)
    
    Renderable(bool visible = true, int layer = 0) : visible(visible), layer(layer) {}
    
    // Component interface
    Component* Clone() const override {
        return new Renderable(visible, layer);
    }
    
    const char* GetTypeName() const override {
        return "Renderable";
    }
    
    static const char* GetTypeNameStatic() {
        return "Renderable";
    }
};

} // namespace ECS
} // namespace Lite2D
