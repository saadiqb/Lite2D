#pragma once

namespace Lite2D {
namespace ECS {

/**
 * Base Component interface
 * All components must inherit from this class
 */
class Component {
public:
    virtual ~Component() = default;
    
    // Virtual clone method for component copying
    virtual Component* Clone() const = 0;
    
    // Component type identification
    virtual const char* GetTypeName() const = 0;
};

} // namespace ECS
} // namespace Lite2D
