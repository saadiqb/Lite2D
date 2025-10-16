#include "ParticleSystem.h"
#include <cmath>
#include <iostream>

namespace Lite2D {
namespace ECS {

void ParticleSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled) return;
    
    // Update particle lifetimes and remove expired ones
    auto entities = entityManager.GetEntitiesWith<Position, Particle>();
    for (Entity entity : entities) {
        Particle* particle = entityManager.GetComponent<Particle>(entity);
        if (particle) {
            particle->UpdateLifetime(deltaTime);
        }
    }
    
    RemoveExpiredParticles(entityManager);
    UpdateStatistics(entityManager);
    
    // Auto-spawning
    if (mAutoSpawnEnabled && mActiveParticleCount < mMaxParticles) {
        mAutoSpawnTimer += deltaTime;
        if (mAutoSpawnTimer >= mAutoSpawnInterval) {
            SpawnRandomParticle(entityManager);
            mAutoSpawnTimer = 0.0f;
        }
    }
}

void ParticleSystem::Initialize(EntityManager& entityManager) {
    std::cout << "ParticleSystem initialized" << std::endl;
    mRandomGenerator = std::mt19937(mRandomDevice());
}

void ParticleSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "ParticleSystem shutdown" << std::endl;
    ClearAllParticles(entityManager);
}

void ParticleSystem::SpawnParticle(EntityManager& entityManager, float x, float y, 
                                  float velX, float velY, float radius, float mass) {
    Entity entity = entityManager.CreateEntity();
    
    // Add components
    entityManager.AddComponent(entity, Position(x, y));
    entityManager.AddComponent(entity, Velocity(velX, velY));
    entityManager.AddComponent(entity, Renderable(true, 1));
    
    // Create particle with random color
    Particle particle(radius, mass, GetRandomFloat(mMinLifetime, mMaxLifetime),
                     GetRandomUint8(mMinR, mMaxR), GetRandomUint8(mMinG, mMaxG),
                     GetRandomUint8(mMinB, mMaxB), GetRandomUint8(mMinA, mMaxA));
    entityManager.AddComponent(entity, particle);
    
    mTotalParticlesSpawned++;
}

void ParticleSystem::SpawnRandomParticle(EntityManager& entityManager) {
    float x = GetRandomFloat(mSpawnMinX, mSpawnMaxX);
    float y = GetRandomFloat(mSpawnMinY, mSpawnMaxY);
    float velX = GetRandomFloat(-mMaxVelocity, mMaxVelocity);
    float velY = GetRandomFloat(-mMaxVelocity, mMaxVelocity);
    float radius = GetRandomFloat(mMinRadius, mMaxRadius);
    float mass = GetRandomFloat(mMinMass, mMaxMass);
    
    SpawnParticle(entityManager, x, y, velX, velY, radius, mass);
}

void ParticleSystem::SpawnParticleBurst(EntityManager& entityManager, int count, float centerX, float centerY) {
    for (int i = 0; i < count; ++i) {
        // Spawn particles in a circular pattern around the center
        float angle = GetRandomFloat(0.0f, 2.0f * M_PI);
        float distance = GetRandomFloat(0.0f, 50.0f);
        float x = centerX + std::cos(angle) * distance;
        float y = centerY + std::sin(angle) * distance;
        
        float velX = std::cos(angle) * GetRandomFloat(mMinVelocity, mMaxVelocity);
        float velY = std::sin(angle) * GetRandomFloat(mMinVelocity, mMaxVelocity);
        
        float radius = GetRandomFloat(mMinRadius, mMaxRadius);
        float mass = GetRandomFloat(mMinMass, mMaxMass);
        
        SpawnParticle(entityManager, x, y, velX, velY, radius, mass);
    }
}

void ParticleSystem::RemoveExpiredParticles(EntityManager& entityManager) {
    auto entities = entityManager.GetEntitiesWith<Particle>();
    std::vector<Entity> toRemove;
    
    for (Entity entity : entities) {
        Particle* particle = entityManager.GetComponent<Particle>(entity);
        if (particle && particle->IsExpired()) {
            toRemove.push_back(entity);
        }
    }
    
    for (Entity entity : toRemove) {
        entityManager.DestroyEntity(entity);
    }
}

void ParticleSystem::ClearAllParticles(EntityManager& entityManager) {
    auto entities = entityManager.GetEntitiesWith<Particle>();
    for (Entity entity : entities) {
        entityManager.DestroyEntity(entity);
    }
    mActiveParticleCount = 0;
}

void ParticleSystem::SetSpawnArea(float minX, float minY, float maxX, float maxY) {
    mSpawnMinX = minX;
    mSpawnMinY = minY;
    mSpawnMaxX = maxX;
    mSpawnMaxY = maxY;
}

void ParticleSystem::SetVelocityRange(float minVel, float maxVel) {
    mMinVelocity = minVel;
    mMaxVelocity = maxVel;
}

void ParticleSystem::SetSizeRange(float minRadius, float maxRadius) {
    mMinRadius = minRadius;
    mMaxRadius = maxRadius;
}

void ParticleSystem::SetMassRange(float minMass, float maxMass) {
    mMinMass = minMass;
    mMaxMass = maxMass;
}

void ParticleSystem::SetLifetimeRange(float minLifetime, float maxLifetime) {
    mMinLifetime = minLifetime;
    mMaxLifetime = maxLifetime;
}

void ParticleSystem::SetColorRange(Uint8 minR, Uint8 maxR, Uint8 minG, Uint8 maxG, 
                                  Uint8 minB, Uint8 maxB, Uint8 minA, Uint8 maxA) {
    mMinR = minR; mMaxR = maxR;
    mMinG = minG; mMaxG = maxG;
    mMinB = minB; mMaxB = maxB;
    mMinA = minA; mMaxA = maxA;
}

void ParticleSystem::EnableAutoSpawn(bool enable, float interval) {
    mAutoSpawnEnabled = enable;
    mAutoSpawnInterval = interval;
    mAutoSpawnTimer = 0.0f;
}

void ParticleSystem::ResetStatistics() {
    mActiveParticleCount = 0;
    mTotalParticlesSpawned = 0;
}

float ParticleSystem::GetRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(mRandomGenerator);
}

int ParticleSystem::GetRandomInt(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(mRandomGenerator);
}

Uint8 ParticleSystem::GetRandomUint8(Uint8 min, Uint8 max) {
    std::uniform_int_distribution<Uint8> distribution(min, max);
    return distribution(mRandomGenerator);
}

void ParticleSystem::UpdateStatistics(EntityManager& entityManager) {
    auto entities = entityManager.GetEntitiesWith<Particle>();
    mActiveParticleCount = 0;
    
    for (Entity entity : entities) {
        Particle* particle = entityManager.GetComponent<Particle>(entity);
        if (particle && particle->isActive) {
            mActiveParticleCount++;
        }
    }
}

} // namespace ECS
} // namespace Lite2D
