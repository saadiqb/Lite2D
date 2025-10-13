#pragma once

#include "EntityManager.h"

namespace Lite2D {
namespace ECS {

/**
 * Base System interface
 * All systems must inherit from this class
 */
class System {
public:
    System() = default;
    virtual ~System() = default;
    
    // Delete copy constructor and assignment operator
    System(const System&) = delete;
    System& operator=(const System&) = delete;
    
    // Main system update function - called every frame
    virtual void Update(EntityManager& entityManager, float deltaTime) = 0;
    
    // System initialization - called when system is registered
    virtual void Initialize(EntityManager& entityManager) {}
    
    // System cleanup - called when system is destroyed
    virtual void Shutdown(EntityManager& entityManager) {}
    
    // Get system name for debugging
    virtual const char* GetName() const = 0;
    
    // Enable/disable system
    void SetEnabled(bool enabled) { mEnabled = enabled; }
    bool IsEnabled() const { return mEnabled; }

protected:
    bool mEnabled = true;
};

} // namespace ECS
} // namespace Lite2D
