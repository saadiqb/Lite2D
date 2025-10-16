#pragma once

#include "ECS/Component.h"
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Particle component for colliding particles animation
 * Contains physical properties and visual attributes
 */
class Particle : public Component {
public:
    float radius;           // Particle radius
    float mass;            // Particle mass for physics calculations
    float lifetime;        // How long the particle lives (seconds)
    float maxLifetime;     // Maximum lifetime
    Uint8 r, g, b, a;     // Color components (RGBA)
    bool isActive;         // Whether the particle is active
    int collisionCount;    // Number of collisions for stress testing
    
    Particle(float radius = 10.0f, float mass = 1.0f, float lifetime = -1.0f, 
             Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255) 
        : radius(radius), mass(mass), lifetime(lifetime), maxLifetime(lifetime),
          r(r), g(g), b(b), a(a), isActive(true), collisionCount(0) {}
    
    // Component interface
    Component* Clone() const override {
        return new Particle(radius, mass, lifetime, r, g, b, a);
    }
    
    const char* GetTypeName() const override {
        return "Particle";
    }
    
    static const char* GetTypeNameStatic() {
        return "Particle";
    }
    
    // Helper methods
    void UpdateLifetime(float deltaTime) {
        if (lifetime > 0.0f) {
            lifetime -= deltaTime;
            if (lifetime <= 0.0f) {
                isActive = false;
            }
        }
    }
    
    void ResetLifetime() {
        lifetime = maxLifetime;
        isActive = true;
    }
    
    bool IsExpired() const {
        return lifetime <= 0.0f && maxLifetime > 0.0f;
    }
};

} // namespace ECS
} // namespace Lite2D
