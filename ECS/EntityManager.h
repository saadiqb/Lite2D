#pragma once

#include "Entity.h"
#include "Component.h"
#include "IComponentArray.h"
#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <bitset>
#include <vector>

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

} // namespace ECS
} // namespace Lite2D

// Include template implementation
#include "EntityManager.inl"
