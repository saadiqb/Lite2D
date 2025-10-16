#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"
#include "../Components/Particle.h"
#include <vector>
#include <random>
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Particle System
 * Manages particle lifecycle, physics, and spawning
 */
class ParticleSystem : public System {
public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "ParticleSystem"; }
    
    // Particle management
    void SpawnParticle(EntityManager& entityManager, float x, float y, 
                      float velX = 0.0f, float velY = 0.0f, 
                      float radius = 10.0f, float mass = 1.0f);
    void SpawnRandomParticle(EntityManager& entityManager);
    void SpawnParticleBurst(EntityManager& entityManager, int count, float centerX, float centerY);
    void RemoveExpiredParticles(EntityManager& entityManager);
    void ClearAllParticles(EntityManager& entityManager);
    
    // Configuration
    void SetSpawnArea(float minX, float minY, float maxX, float maxY);
    void SetVelocityRange(float minVel, float maxVel);
    void SetSizeRange(float minRadius, float maxRadius);
    void SetMassRange(float minMass, float maxMass);
    void SetLifetimeRange(float minLifetime, float maxLifetime);
    void SetColorRange(Uint8 minR, Uint8 maxR, Uint8 minG, Uint8 maxG, 
                      Uint8 minB, Uint8 maxB, Uint8 minA, Uint8 maxA);
    
    // Auto-spawning
    void EnableAutoSpawn(bool enable, float interval = 1.0f);
    void SetMaxParticles(int maxParticles) { mMaxParticles = maxParticles; }
    
    // Statistics
    int GetActiveParticleCount() const { return mActiveParticleCount; }
    int GetTotalParticlesSpawned() const { return mTotalParticlesSpawned; }
    void ResetStatistics();

private:
    // Spawn configuration
    float mSpawnMinX = 100.0f, mSpawnMinY = 100.0f;
    float mSpawnMaxX = 800.0f, mSpawnMaxY = 600.0f;
    float mMinVelocity = 50.0f, mMaxVelocity = 200.0f;
    float mMinRadius = 5.0f, mMaxRadius = 25.0f;
    float mMinMass = 0.5f, mMaxMass = 3.0f;
    float mMinLifetime = 10.0f, mMaxLifetime = 30.0f;
    
    // Color range
    Uint8 mMinR = 100, mMaxR = 255;
    Uint8 mMinG = 100, mMaxG = 255;
    Uint8 mMinB = 100, mMaxB = 255;
    Uint8 mMinA = 150, mMaxA = 255;
    
    // Auto-spawning
    bool mAutoSpawnEnabled = false;
    float mAutoSpawnInterval = 1.0f;
    float mAutoSpawnTimer = 0.0f;
    int mMaxParticles = 1000;
    
    // Statistics
    int mActiveParticleCount = 0;
    int mTotalParticlesSpawned = 0;
    
    // Random number generation
    std::random_device mRandomDevice;
    std::mt19937 mRandomGenerator;
    
    // Helper functions
    float GetRandomFloat(float min, float max);
    int GetRandomInt(int min, int max);
    Uint8 GetRandomUint8(Uint8 min, Uint8 max);
    void UpdateStatistics(EntityManager& entityManager);
};

} // namespace ECS
} // namespace Lite2D
