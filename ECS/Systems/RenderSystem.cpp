#include "RenderSystem.h"
#include "../Components/Velocity.h"
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
    
    // Set up debug text rendering parameters
    const int DEBUG_FONT_SIZE = 16;
    const int DEBUG_LINE_HEIGHT = 20;
    const int DEBUG_START_X = 10;
    const int DEBUG_START_Y = 10;
    
    // Debug text color (bright green)
    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
    
    // Get debug information
    size_t totalEntities = entityManager.GetEntityCount();
    auto entitiesWithPosition = entityManager.GetEntitiesWith<Position>();
    auto entitiesWithRenderable = entityManager.GetEntitiesWith<Renderable>();
    auto entitiesWithVelocity = entityManager.GetEntitiesWith<Velocity>();
    auto entitiesWithBoth = entityManager.GetEntitiesWith<Position, Renderable>();
    
    // Render debug info background
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 128); // Semi-transparent black
    SDL_FRect debugBg = {5, 5, 350, 200};
    SDL_RenderFillRect(mRenderer, &debugBg);
    
    // Debug info border
    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255); // Green border
    SDL_RenderRect(mRenderer, &debugBg);
    
    // Reset text color
    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
    
    // Helper function to render a debug line (simplified text rendering)
    auto RenderDebugLine = [this, DEBUG_START_X, DEBUG_START_Y, DEBUG_LINE_HEIGHT](int line, const std::string& text) {
        // For now, we'll render simple rectangles to represent text
        // In a real implementation, you'd use a font rendering system
        int y = DEBUG_START_Y + (line * DEBUG_LINE_HEIGHT);
        
        // Render a small indicator rectangle for each line
        SDL_FRect indicator = {static_cast<float>(DEBUG_START_X), static_cast<float>(y + 5), 10.0f, 10.0f};
        SDL_RenderFillRect(mRenderer, &indicator);
        
        // Note: Actual text rendering would require SDL_ttf or similar
        // For now, we'll just render the indicator rectangles
    };
    
    // Render debug information lines
    RenderDebugLine(0, "=== ECS Debug Info ===");
    RenderDebugLine(1, "Total Entities: " + std::to_string(totalEntities));
    RenderDebugLine(2, "With Position: " + std::to_string(entitiesWithPosition.size()));
    RenderDebugLine(3, "With Renderable: " + std::to_string(entitiesWithRenderable.size()));
    RenderDebugLine(4, "With Velocity: " + std::to_string(entitiesWithVelocity.size()));
    RenderDebugLine(5, "Renderable (Pos+Ren): " + std::to_string(entitiesWithBoth.size()));
    
    // Also print debug info to console for demonstration
    static int debugCounter = 0;
    if (debugCounter++ % 60 == 0) { // Print every 60 frames (roughly once per second)
        std::cout << "[DEBUG] ECS Stats - Entities: " << totalEntities 
                  << ", Pos: " << entitiesWithPosition.size()
                  << ", Ren: " << entitiesWithRenderable.size()
                  << ", Vel: " << entitiesWithVelocity.size()
                  << ", RenderItems: " << mRenderItems.size() << std::endl;
    }
    
    // Render camera info
    RenderDebugLine(6, "Camera Offset: (" + 
                    std::to_string((int)mCameraOffsetX) + ", " + 
                    std::to_string((int)mCameraOffsetY) + ")");
    
    // Render render items info
    RenderDebugLine(7, "Render Items: " + std::to_string(mRenderItems.size()));
    
    // Render layer info if we have render items
    if (!mRenderItems.empty()) {
        int minLayer = mRenderItems[0].renderable->layer;
        int maxLayer = mRenderItems[0].renderable->layer;
        
        for (const auto& item : mRenderItems) {
            minLayer = std::min(minLayer, item.renderable->layer);
            maxLayer = std::max(maxLayer, item.renderable->layer);
        }
        
        RenderDebugLine(8, "Layer Range: " + std::to_string(minLayer) + " - " + std::to_string(maxLayer));
    }
    
    // Render entity bounding boxes if we have render items
    if (mShowDebugInfo && !mRenderItems.empty()) {
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 128); // Semi-transparent red
        
        for (const auto& item : mRenderItems) {
            if (!item.position || !item.renderable) continue;
            
            // Apply camera offset
            float screenX = item.position->x - mCameraOffsetX;
            float screenY = item.position->y - mCameraOffsetY;
            
            // Render entity bounding box
            SDL_FRect bbox = {screenX - 10, screenY - 10, 20, 20};
            SDL_RenderRect(mRenderer, &bbox);
            
            // Render entity ID at the center (simplified)
            SDL_FRect idBox = {screenX - 2, screenY - 2, 4, 4};
            SDL_RenderFillRect(mRenderer, &idBox);
        }
    }
}

} // namespace ECS
} // namespace Lite2D
