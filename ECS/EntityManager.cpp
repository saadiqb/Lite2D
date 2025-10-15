#include "EntityManager.h"
#include <algorithm>
#include <vector>

namespace Lite2D {
namespace ECS {

EntityManager::EntityManager() 
    : mLivingEntityCount(0), mNextComponentType(0) {
    // Initialize the queue with all possible entity IDs
    for (Entity entity = 1; entity < MAX_ENTITIES; ++entity) {
        mAvailableEntities.push_back(entity);
    }
}

Entity EntityManager::CreateEntity() {
    if (mAvailableEntities.empty()) {
        // No more entities available
        return INVALID_ENTITY;
    }
    
    // Take an ID from the front of the queue
    Entity id = mAvailableEntities.front();
    mAvailableEntities.erase(mAvailableEntities.begin());
    
    // Add to active entities list for performance optimization
    mActiveEntities.push_back(id);
    
    mLivingEntityCount++;
    
    return id;
}

void EntityManager::DestroyEntity(Entity entity) {
    if (!IsValid(entity)) {
        return;
    }
    
    // Remove from active entities list for performance optimization
    auto it = std::find(mActiveEntities.begin(), mActiveEntities.end(), entity);
    if (it != mActiveEntities.end()) {
        mActiveEntities.erase(it);
    }
    
    // Invalidate the destroyed entity's signature
    mEntitySignatures[entity].reset();
    
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto& componentArray : mComponentArrays) {
        if (componentArray) {
            componentArray->EntityDestroyed(entity);
        }
    }
    
    // Put the destroyed entity's ID back into the available entities queue
    mAvailableEntities.push_back(entity);
    
    mLivingEntityCount--;
}

bool EntityManager::IsValid(Entity entity) const {
    // Check if entity ID is within valid range
    if (entity == INVALID_ENTITY || entity >= MAX_ENTITIES) {
        return false;
    }
    
    // Check if entity is in the available entities list
    // If it's available, it means it's not currently in use
    auto it = std::find(mAvailableEntities.begin(), mAvailableEntities.end(), entity);
    return it == mAvailableEntities.end();
}

void EntityManager::Clear() {
    // Clear all signatures
    for (auto& signature : mEntitySignatures) {
        signature.reset();
    }
    
    // Clear all component arrays
    for (auto& componentArray : mComponentArrays) {
        if (componentArray) {
            componentArray.reset();
        }
    }
    
    // Clear active entities list
    mActiveEntities.clear();
    
    // Reset available entities queue
    mAvailableEntities.clear();
    for (Entity entity = 1; entity < MAX_ENTITIES; ++entity) {
        mAvailableEntities.push_back(entity);
    }
    
    mLivingEntityCount = 0;
    mNextComponentType = 0;
    mComponentTypes.clear();
    mComponentNames.clear();
}

void EntityManager::SetSignature(Entity entity, std::bitset<MAX_COMPONENT_TYPES> signature) {
    mEntitySignatures[entity] = signature;
}

std::bitset<MAX_COMPONENT_TYPES> EntityManager::GetSignature(Entity entity) const {
    return mEntitySignatures[entity];
}

} // namespace ECS
} // namespace Lite2D
