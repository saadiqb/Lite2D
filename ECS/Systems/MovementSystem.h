#pragma once

#include "../System.h"
#include "../EntityManager.h"
#include "../Components/Position.h"
#include "../Components/Velocity.h"

namespace Lite2D {
namespace ECS {

/**
 * Movement System
 * Updates entity positions based on their velocity components
 */
class MovementSystem : public System {
public:
    MovementSystem() = default;
    ~MovementSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "MovementSystem"; }
    
    // System-specific configuration
    void SetMaxSpeed(float maxSpeed) { mMaxSpeed = maxSpeed; }
    float GetMaxSpeed() const { return mMaxSpeed; }
    
    // Boundary checking
    void SetBoundaries(float minX, float minY, float maxX, float maxY);
    void EnableBoundaryClamping(bool enable) { mClampToBoundaries = enable; }

private:
    float mMaxSpeed = 1000.0f; // Maximum speed units per second
    bool mClampToBoundaries = false;
    float mMinX = 0.0f, mMinY = 0.0f, mMaxX = 1920.0f, mMaxY = 1080.0f;
    
    void ClampPosition(Position& position);
};

} // namespace ECS
} // namespace Lite2D
