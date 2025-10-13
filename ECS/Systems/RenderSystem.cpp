#include "RenderSystem.h"
#include <iostream>
#include <algorithm>

namespace Lite2D {
namespace ECS {

RenderSystem::RenderSystem(SDL_Renderer* renderer) : mRenderer(renderer) {
    if (!mRenderer) {
        std::cerr << "Warning: RenderSystem created with null renderer" << std::endl;
    }
}

void RenderSystem::Update(EntityManager& entityManager, float deltaTime) {
    if (!mEnabled || !mRenderer) return;
    
    // Clear the screen
    ClearScreen();
    
    // Get all entities with both Position and Renderable components
    auto entities = entityManager.GetEntitiesWith<Position, Renderable>();
    
    // Collect render items
    mRenderItems.clear();
    mRenderItems.reserve(entities.size());
    
    for (Entity entity : entities) {
        Position* position = entityManager.GetComponent<Position>(entity);
        Renderable* renderable = entityManager.GetComponent<Renderable>(entity);
        
        if (!position || !renderable || !renderable->visible) continue;
        
        mRenderItems.push_back({entity, position, renderable});
    }
    
    // Sort by render layer
    std::sort(mRenderItems.begin(), mRenderItems.end(), 
        [this](const RenderItem& a, const RenderItem& b) {
            if (mRenderAscending) {
                return a.renderable->layer < b.renderable->layer;
            } else {
                return a.renderable->layer > b.renderable->layer;
            }
        });
    
    // Render all entities
    for (const auto& item : mRenderItems) {
        RenderEntity(item.position, item.renderable);
    }
    
    // Render debug info if enabled
    if (mShowDebugInfo) {
        RenderDebugInfo(entityManager);
    }
    
    // Present the frame
    PresentFrame();
}

void RenderSystem::Initialize(EntityManager& entityManager) {
    std::cout << "RenderSystem initialized" << std::endl;
}

void RenderSystem::Shutdown(EntityManager& entityManager) {
    std::cout << "RenderSystem shutdown" << std::endl;
}

void RenderSystem::SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (mRenderer) {
        SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
    }
}

void RenderSystem::SetCamera(float offsetX, float offsetY) {
    mCameraOffsetX = offsetX;
    mCameraOffsetY = offsetY;
}

void RenderSystem::GetCamera(float& offsetX, float& offsetY) const {
    offsetX = mCameraOffsetX;
    offsetY = mCameraOffsetY;
}

void RenderSystem::ClearScreen() {
    if (mRenderer) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(mRenderer);
    }
}

void RenderSystem::PresentFrame() {
    if (mRenderer) {
        SDL_RenderPresent(mRenderer);
    }
}

void RenderSystem::RenderEntity(Position* position, Renderable* renderable) {
    if (!mRenderer || !position || !renderable) return;
    
    // Apply camera offset
    float screenX = position->x - mCameraOffsetX;
    float screenY = position->y - mCameraOffsetY;
    
    // For now, render a simple rectangle
    // In a real implementation, this would render sprites, textures, etc.
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // White color
    
    // Render a 20x20 rectangle at the entity's position
    SDL_FRect rect = {screenX - 10, screenY - 10, 20, 20};
    SDL_RenderFillRect(mRenderer, &rect);
    
    // Render a border to distinguish different layers
    Uint8 borderColor = 255 - (renderable->layer * 30);
    SDL_SetRenderDrawColor(mRenderer, borderColor, borderColor, borderColor, 255);
    SDL_RenderRect(mRenderer, &rect);
}

void RenderSystem::RenderDebugInfo(EntityManager& entityManager) {
    if (!mRenderer) return;
    
    // This is a placeholder for debug rendering
    // In a real implementation, you might render:
    // - Entity count
    // - FPS
    // - Component counts
    // - System performance metrics
}

} // namespace ECS
} // namespace Lite2D
