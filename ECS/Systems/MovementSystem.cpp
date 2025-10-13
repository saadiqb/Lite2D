#include "MovementSystem.h"
#include <iostream>

namespace Lite2D {
namespace ECS {

void MovementSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled) return;
    
    // Get all entities with both Position and Velocity components
    auto entities = entityManager.GetEntitiesWith<Position, Velocity>();
    
    for (Entity entity : entities) {
        Position* position = entityManager.GetComponent<Position>(entity);
        Velocity* velocity = entityManager.GetComponent<Velocity>(entity);
        
        if (!position || !velocity) continue;
        
        // Update position based on velocity and delta time
        position->x += velocity->x * deltaTime;
        position->y += velocity->y * deltaTime;
        
        // Apply speed limiting if configured
        float speed = sqrt(velocity->x * velocity->x + velocity->y * velocity->y);
        if (speed > mMaxSpeed && speed > 0.0f) {
            float scale = mMaxSpeed / speed;
            velocity->x *= scale;
            velocity->y *= scale;
        }
        
        // Apply boundary clamping if enabled
        if (mClampToBoundaries) {
            ClampPosition(*position);
        }
    }
}

void MovementSystem::Initialize(EntityManager& entityManager) {
    std::cout << "MovementSystem initialized" << std::endl;
}

void MovementSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "MovementSystem shutdown" << std::endl;
}

void MovementSystem::SetBoundaries(float minX, float minY, float maxX, float maxY) {
    mMinX = minX;
    mMinY = minY;
    mMaxX = maxX;
    mMaxY = maxY;
}

void MovementSystem::ClampPosition(Position& position) {
    if (position.x < mMinX) position.x = mMinX;
    if (position.x > mMaxX) position.x = mMaxX;
    if (position.y < mMinY) position.y = mMinY;
    if (position.y > mMaxY) position.y = mMaxY;
}

} // namespace ECS
} // namespace Lite2D
