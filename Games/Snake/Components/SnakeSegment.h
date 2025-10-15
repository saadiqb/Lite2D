#pragma once

#include "ECS/Component.h"

namespace Lite2D {
namespace ECS {

/**
 * Snake Segment component
 * Represents a single segment of the snake's body
 */
class SnakeSegment : public Component {
public:
    int segmentIndex; // Position in the snake (0 = head, 1+ = body)
    
    SnakeSegment(int index = 0) : segmentIndex(index) {}
    
    // Component interface
    Component* Clone() const override {
        return new SnakeSegment(segmentIndex);
    }
    
    const char* GetTypeName() const override {
        return "SnakeSegment";
    }
    
    static const char* GetTypeNameStatic() {
        return "SnakeSegment";
    }
};

} // namespace ECS
} // namespace Lite2D
