#pragma once

#include "Entity.h"

namespace Lite2D {
namespace ECS {

// Forward declaration
class Component;

/**
 * Interface for component storage arrays
 * Allows type erasure for storing different component types
 */
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    
    // Add component to entity
    virtual void InsertData(Entity entity, Component* component) = 0;
    
    // Remove component from entity
    virtual void RemoveData(Entity entity) = 0;
    
    // Get component from entity
    virtual Component* GetData(Entity entity) = 0;
    
    // Check if entity has this component
    virtual bool HasData(Entity entity) const = 0;
    
    // Called when entity is destroyed
    virtual void EntityDestroyed(Entity entity) = 0;
    
    // Get component type name
    virtual const char* GetComponentTypeName() const = 0;
};

} // namespace ECS
} // namespace Lite2D
