#include "CollisionSystem.h"
#include <cmath>
#include <iostream>

namespace Lite2D {
namespace ECS {

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled) return;
    
    mCollisionCount = 0;
    UpdateParticleList(entityManager);
    
    // Check particle-to-particle collisions
    for (size_t i = 0; i < mParticleEntities.size(); ++i) {
        for (size_t j = i + 1; j < mParticleEntities.size(); ++j) {
            Entity entity1 = mParticleEntities[i];
            Entity entity2 = mParticleEntities[j];
            
            if (CheckParticleCollision(entityManager, entity1, entity2)) {
                ResolveParticleCollision(entityManager, entity1, entity2);
                mCollisionCount++;
            }
        }
    }
    
    // Check boundary collisions
    for (Entity entity : mParticleEntities) {
        if (CheckBoundaryCollision(entityManager, entity)) {
            ResolveBoundaryCollision(entityManager, entity);
        }
    }
}

void CollisionSystem::Initialize(EntityManager& entityManager) {
    std::cout << "CollisionSystem initialized" << std::endl;
}

void CollisionSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "CollisionSystem shutdown" << std::endl;
    mParticleEntities.clear();
}

void CollisionSystem::SetBoundaries(float minX, float minY, float maxX, float maxY) {
    mMinX = minX;
    mMinY = minY;
    mMaxX = maxX;
    mMaxY = maxY;
}

void CollisionSystem::UpdateParticleList(EntityManager& entityManager) {
    mParticleEntities.clear();
    auto entities = entityManager.GetEntitiesWith<Position, Velocity, Particle>();
    
    for (Entity entity : entities) {
        Particle* particle = entityManager.GetComponent<Particle>(entity);
        if (particle && particle->isActive) {
            mParticleEntities.push_back(entity);
        }
    }
}

bool CollisionSystem::CheckParticleCollision(EntityManager& entityManager, Entity entity1, Entity entity2) {
    Position* pos1 = entityManager.GetComponent<Position>(entity1);
    Position* pos2 = entityManager.GetComponent<Position>(entity2);
    Particle* particle1 = entityManager.GetComponent<Particle>(entity1);
    Particle* particle2 = entityManager.GetComponent<Particle>(entity2);
    
    if (!pos1 || !pos2 || !particle1 || !particle2) {
        return false;
    }
    
    float distance = CalculateDistance(*pos1, *pos2);
    float minDistance = particle1->radius + particle2->radius;
    
    return distance <= minDistance;
}

bool CollisionSystem::CheckBoundaryCollision(EntityManager& entityManager, Entity entity) {
    Position* pos = entityManager.GetComponent<Position>(entity);
    Particle* particle = entityManager.GetComponent<Particle>(entity);
    
    if (!pos || !particle) {
        return false;
    }
    
    return (pos->x - particle->radius <= mMinX) || 
           (pos->x + particle->radius >= mMaxX) ||
           (pos->y - particle->radius <= mMinY) || 
           (pos->y + particle->radius >= mMaxY);
}

void CollisionSystem::ResolveParticleCollision(EntityManager& entityManager, Entity entity1, Entity entity2) {
    Position* pos1 = entityManager.GetComponent<Position>(entity1);
    Position* pos2 = entityManager.GetComponent<Position>(entity2);
    Velocity* vel1 = entityManager.GetComponent<Velocity>(entity1);
    Velocity* vel2 = entityManager.GetComponent<Velocity>(entity2);
    Particle* particle1 = entityManager.GetComponent<Particle>(entity1);
    Particle* particle2 = entityManager.GetComponent<Particle>(entity2);
    
    if (!pos1 || !pos2 || !vel1 || !vel2 || !particle1 || !particle2) {
        return;
    }
    
    // Calculate collision normal
    float dx = pos2->x - pos1->x;
    float dy = pos2->y - pos1->y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance == 0.0f) {
        // Handle edge case where particles are exactly on top of each other
        dx = (rand() % 2 == 0) ? 1.0f : -1.0f;
        dy = (rand() % 2 == 0) ? 1.0f : -1.0f;
        distance = std::sqrt(dx * dx + dy * dy);
    }
    
    float normalX = dx / distance;
    float normalY = dy / distance;
    
    // Separate particles to prevent overlap
    float overlap = (particle1->radius + particle2->radius) - distance;
    float separationX = normalX * overlap * 0.5f;
    float separationY = normalY * overlap * 0.5f;
    
    pos1->x -= separationX;
    pos1->y -= separationY;
    pos2->x += separationX;
    pos2->y += separationY;
    
    // Calculate relative velocity
    float relativeVelX = vel2->x - vel1->x;
    float relativeVelY = vel2->y - vel1->y;
    
    // Calculate relative velocity along collision normal
    float velocityAlongNormal = relativeVelX * normalX + relativeVelY * normalY;
    
    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) {
        return;
    }
    
    // Calculate restitution (elasticity)
    float restitution = mElasticity;
    
    // Calculate impulse scalar
    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (1.0f / particle1->mass + 1.0f / particle2->mass);
    
    // Apply impulse
    ApplyImpulse(*vel1, *vel2, particle1->mass, particle2->mass, normalX, normalY, impulseScalar);
    
    // Update collision counts for stress testing
    particle1->collisionCount++;
    particle2->collisionCount++;
}

void CollisionSystem::ResolveBoundaryCollision(EntityManager& entityManager, Entity entity) {
    Position* pos = entityManager.GetComponent<Position>(entity);
    Velocity* vel = entityManager.GetComponent<Velocity>(entity);
    Particle* particle = entityManager.GetComponent<Particle>(entity);
    
    if (!pos || !vel || !particle) {
        return;
    }
    
    // Check and resolve left/right boundaries
    if (pos->x - particle->radius <= mMinX) {
        pos->x = mMinX + particle->radius;
        vel->x = -vel->x * mElasticity;
    } else if (pos->x + particle->radius >= mMaxX) {
        pos->x = mMaxX - particle->radius;
        vel->x = -vel->x * mElasticity;
    }
    
    // Check and resolve top/bottom boundaries
    if (pos->y - particle->radius <= mMinY) {
        pos->y = mMinY + particle->radius;
        vel->y = -vel->y * mElasticity;
    } else if (pos->y + particle->radius >= mMaxY) {
        pos->y = mMaxY - particle->radius;
        vel->y = -vel->y * mElasticity;
    }
    
    // Apply friction
    vel->x *= (1.0f - mFriction);
    vel->y *= (1.0f - mFriction);
    
    // Update collision count
    particle->collisionCount++;
}

float CollisionSystem::CalculateDistance(const Position& pos1, const Position& pos2) {
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return std::sqrt(dx * dx + dy * dy);
}

void CollisionSystem::ApplyImpulse(Velocity& vel1, Velocity& vel2, float mass1, float mass2, 
                                  float normalX, float normalY, float impulse) {
    // Apply impulse to both velocities
    float impulseX = impulse * normalX;
    float impulseY = impulse * normalY;
    
    vel1.x -= impulseX / mass1;
    vel1.y -= impulseY / mass1;
    vel2.x += impulseX / mass2;
    vel2.y += impulseY / mass2;
}

} // namespace ECS
} // namespace Lite2D
