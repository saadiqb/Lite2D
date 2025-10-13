#pragma once

#include "../System.h"
#include "../EntityManager.h"
#include "../Components/Position.h"
#include "../Components/Renderable.h"
#include <SDL3/SDL.h>

namespace Lite2D {
namespace ECS {

/**
 * Render System
 * Renders entities with Position and Renderable components
 */
class RenderSystem : public System {
public:
    RenderSystem(SDL_Renderer* renderer);
    ~RenderSystem() = default;
    
    // System interface
    void Update(EntityManager& entityManager, float deltaTime) override;
    void Initialize(EntityManager& entityManager) override;
    void Shutdown(EntityManager& entityManager) override;
    const char* GetName() const override { return "RenderSystem"; }
    
    // Rendering configuration
    void SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void SetRenderOrder(bool ascending = true) { mRenderAscending = ascending; }
    void EnableDebugInfo(bool enable) { mShowDebugInfo = enable; }
    
    // Camera/viewport
    void SetCamera(float offsetX, float offsetY);
    void GetCamera(float& offsetX, float& offsetY) const;

private:
    SDL_Renderer* mRenderer;
    bool mRenderAscending = true; // true = lower layers first, false = higher layers first
    bool mShowDebugInfo = false;
    float mCameraOffsetX = 0.0f;
    float mCameraOffsetY = 0.0f;
    
    // Rendering helpers
    void ClearScreen();
    void PresentFrame();
    void RenderEntity(Position* position, Renderable* renderable);
    void RenderDebugInfo(EntityManager& entityManager);
    
    struct RenderItem {
        Entity entity;
        Position* position;
        Renderable* renderable;
    };
    
    std::vector<RenderItem> mRenderItems;
};

} // namespace ECS
} // namespace Lite2D
