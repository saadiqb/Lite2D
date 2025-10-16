#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "../Components/Particle.h"
#include <vector>

namespace Lite2D {
namespace ECS {

/**
 * Collision System for Particles
 * Handles particle-to-particle collisions and boundary collisions
 */
class CollisionSystem : public System {
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "CollisionSystem"; }
    
    // Configuration
    void SetBoundaries(float minX, float minY, float maxX, float maxY);
    void SetElasticity(float elasticity) { mElasticity = elasticity; }
    void SetFriction(float friction) { mFriction = friction; }
    
    // Statistics
    int GetCollisionCount() const { return mCollisionCount; }
    void ResetCollisionCount() { mCollisionCount = 0; }

private:
    float mMinX = 0.0f, mMinY = 0.0f, mMaxX = 1920.0f, mMaxY = 1080.0f;
    float mElasticity = 0.8f;  // Energy retention after collision (0.0 = inelastic, 1.0 = perfectly elastic)
    float mFriction = 0.1f;    // Friction coefficient
    int mCollisionCount = 0;   // Total collisions this frame
    
    // Collision detection and resolution
    bool CheckParticleCollision(EntityManager& entityManager, Entity entity1, Entity entity2);
    bool CheckBoundaryCollision(EntityManager& entityManager, Entity entity);
    
    void ResolveParticleCollision(EntityManager& entityManager, Entity entity1, Entity entity2);
    void ResolveBoundaryCollision(EntityManager& entityManager, Entity entity);
    
    // Physics helpers
    float CalculateDistance(const Position& pos1, const Position& pos2);
    void ApplyImpulse(Velocity& vel1, Velocity& vel2, float mass1, float mass2, 
                     float normalX, float normalY, float impulse);
    
    // Spatial optimization for collision detection
    std::vector<Entity> mParticleEntities;
    void UpdateParticleList(EntityManager& entityManager);
};

} // namespace ECS
} // namespace Lite2D
