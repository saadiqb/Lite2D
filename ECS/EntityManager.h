#pragma once

#include "Entity.h"
#include "Component.h"
#include "IComponentArray.h"
#include "ComponentArray.h"
#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Lite2D {
namespace ECS {

// Forward declarations
template<typename T>
class ComponentArray;

/**
 * High-performance Entity Manager
 * Handles entity lifecycle and component management
 */
class EntityManager {
public:
    EntityManager();
    ~EntityManager() = default;
    
    // Delete copy constructor and assignment operator
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    
    // Entity lifecycle
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsValid(Entity entity) const;
    
    // Component management
    template<typename T>
    void AddComponent(Entity entity, T component);
    
    template<typename T>
    void RemoveComponent(Entity entity);
    
    template<typename T>
    T* GetComponent(Entity entity);
    
    template<typename T>
    bool HasComponent(Entity entity) const;
    
    // Entity queries
    template<typename... Components>
    std::vector<Entity> GetEntitiesWith();
    
    // Component signature helper
    template<typename... Components>
    std::bitset<MAX_COMPONENT_TYPES> GetComponentSignature();
    
    // Utility
    size_t GetEntityCount() const { return mLivingEntityCount; }
    void Clear();
    
    // Component type registration
    template<typename T>
    void RegisterComponentType();
    
    // Get component array for iteration (advanced usage)
    template<typename T>
    ComponentArray<T>* GetComponentArray();

private:
    // Array of component type arrays
    // Index in this array corresponds to the component type bit in the signature
    std::array<std::unique_ptr<IComponentArray>, MAX_COMPONENT_TYPES> mComponentArrays;
    
    // Map from component type to array index
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;
    
    // Map from array index to component type name
    std::unordered_map<ComponentType, const char*> mComponentNames;
    
    // Array of signatures where the index corresponds to the entity ID
    // Each signature tells us which components the entity has
    std::array<std::bitset<MAX_COMPONENT_TYPES>, MAX_ENTITIES> mEntitySignatures;
    
    // Queue of unused entity IDs
    std::vector<Entity> mAvailableEntities;
    
    // Active entities list - ONLY contains living entities (major performance optimization)
    std::vector<Entity> mActiveEntities;
    
    // Total living entities - used to keep limits on how many exist
    size_t mLivingEntityCount;
    
    // Next component type to be assigned
    ComponentType mNextComponentType;
    
    // Helper functions
    template<typename T>
    ComponentType GetComponentType();
    
    void SetSignature(Entity entity, std::bitset<MAX_COMPONENT_TYPES> signature);
    std::bitset<MAX_COMPONENT_TYPES> GetSignature(Entity entity) const;
};

// Template implementations
template<typename T>
void EntityManager::RegisterComponentType() {
    const char* typeName = typeid(T).name();
    
    // Register this component type
    mComponentTypes.insert({std::type_index(typeid(T)), mNextComponentType});
    mComponentNames.insert({mNextComponentType, typeName});
    
    // Create a ComponentArray pointer and add it to the component arrays array
    mComponentArrays[mNextComponentType] = std::make_unique<ComponentArray<T>>();
    
    // Increment the component type
    mNextComponentType++;
}

template<typename T>
ComponentType EntityManager::GetComponentType() {
    const char* typeName = typeid(T).name();
    
    auto it = mComponentTypes.find(std::type_index(typeid(T)));
    if (it == mComponentTypes.end()) {
        // Component type not registered, register it now
        RegisterComponentType<T>();
        return mNextComponentType - 1;
    }
    
    return it->second;
}

template<typename T>
void EntityManager::AddComponent(Entity entity, T component) {
    // Add a component to the array for an entity
    ComponentType componentType = GetComponentType<T>();
    
    if (!mComponentArrays[componentType]) {
        return;
    }
    
    mComponentArrays[componentType]->InsertData(entity, &component);
    
    // Set this bit to signify that the entity has this component
    std::bitset<MAX_COMPONENT_TYPES> signature = GetSignature(entity);
    signature.set(componentType);
    SetSignature(entity, signature);
}

template<typename T>
void EntityManager::RemoveComponent(Entity entity) {
    // Remove a component from the array for an entity
    ComponentType componentType = GetComponentType<T>();
    mComponentArrays[componentType]->RemoveData(entity);
    
    // Unset this bit to signify that the entity doesn't have this component
    std::bitset<MAX_COMPONENT_TYPES> signature = GetSignature(entity);
    signature.set(componentType, false);
    SetSignature(entity, signature);
}

template<typename T>
T* EntityManager::GetComponent(Entity entity) {
    // Get a reference to a component from the array for an entity
    ComponentType componentType = GetComponentType<T>();
    
    if (!mComponentArrays[componentType]) {
        return nullptr;
    }
    
    auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[componentType].get());
    return componentArray->GetComponent(entity);
}

template<typename T>
bool EntityManager::HasComponent(Entity entity) const {
    // Check if the entity has a component of type T
    ComponentType componentType = const_cast<EntityManager*>(this)->GetComponentType<T>();
    return mComponentArrays[componentType]->HasData(entity);
}

template<typename... Components>
std::vector<Entity> EntityManager::GetEntitiesWith() {
    std::vector<Entity> matchingEntities;
    matchingEntities.reserve(mActiveEntities.size()); // Pre-allocate for performance
    
    // Create a signature that matches all the required components
    std::bitset<MAX_COMPONENT_TYPES> requiredSignature;
    (requiredSignature.set(GetComponentType<Components>()), ...);
    
    // MAJOR PERFORMANCE OPTIMIZATION: Only iterate through active entities
    // This changes complexity from O(MAX_ENTITIES) to O(active_entities)
    for (Entity entity : mActiveEntities) {
        std::bitset<MAX_COMPONENT_TYPES> entitySignature = GetSignature(entity);
        
        // Check if entity has all required components
        if ((entitySignature & requiredSignature) == requiredSignature) {
            matchingEntities.push_back(entity);
        }
    }
    
    return matchingEntities;
}

template<typename T>
ComponentArray<T>* EntityManager::GetComponentArray() {
    ComponentType componentType = GetComponentType<T>();
    return static_cast<ComponentArray<T>*>(mComponentArrays[componentType].get());
}

template<typename... Components>
std::bitset<MAX_COMPONENT_TYPES> EntityManager::GetComponentSignature() {
    std::bitset<MAX_COMPONENT_TYPES> signature;
    (signature.set(GetComponentType<Components>()), ...);
    return signature;
}

} // namespace ECS
} // namespace Lite2D
