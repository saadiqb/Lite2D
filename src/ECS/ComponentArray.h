#pragma once

#include "IComponentArray.h"
#include "Component.h"
#include <array>
#include <iostream>

namespace Lite2D {
namespace ECS {

/**
 * Type-safe component storage array
 * Stores components of type T in contiguous memory for cache-friendly iteration
 */
template<typename T>
class ComponentArray : public IComponentArray {
public:
    // Type traits
    static_assert(std::is_base_of_v<Component, T>, "Component must inherit from Component base class");
    
    ComponentArray() {
        // Initialize sparse array with unused slot marker
        // Use MAX_ENTITIES as the unused slot marker since entities are 1 to MAX_ENTITIES-1
        mEntityToIndex.fill(MAX_ENTITIES);
        mIndexToEntity.fill(INVALID_ENTITY);
    }
    
    // Add component to entity
    void InsertData(Entity entity, Component* component) override {
        if (mEntityToIndex[entity] != MAX_ENTITIES) {
            // Entity already has this component, update it
            mComponentArray[mEntityToIndex[entity]] = *static_cast<T*>(component);
            return;
        }
        
        // Put new entry at end
        size_t newIndex = mSize;
        mEntityToIndex[entity] = newIndex;
        mIndexToEntity[newIndex] = entity;
        mComponentArray[newIndex] = *static_cast<T*>(component);
        mSize++;
    }
    
    // Remove component from entity
    void RemoveData(Entity entity) override {
        if (mEntityToIndex[entity] == MAX_ENTITIES) {
            return; // Entity doesn't have this component
        }
        
        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = mEntityToIndex[entity];
        size_t indexOfLastElement = mSize - 1;
        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];
        
        // Update map to point to moved spot
        Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];
        mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;
        
        mEntityToIndex[entity] = MAX_ENTITIES;
        mIndexToEntity[indexOfLastElement] = INVALID_ENTITY;
        mSize--;
    }
    
    // Get component from entity
    Component* GetData(Entity entity) override {
        if (mEntityToIndex[entity] == MAX_ENTITIES) {
            return nullptr;
        }
        return &mComponentArray[mEntityToIndex[entity]];
    }
    
    // Check if entity has this component
    bool HasData(Entity entity) const override {
        return mEntityToIndex[entity] != MAX_ENTITIES;
    }
    
    // Called when entity is destroyed
    void EntityDestroyed(Entity entity) override {
        if (mEntityToIndex[entity] != MAX_ENTITIES) {
            RemoveData(entity);
        }
    }
    
    // Get component type name
    const char* GetComponentTypeName() const override {
        return T::GetTypeNameStatic();
    }
    
    // Get typed component (for performance)
    T* GetComponent(Entity entity) {
        if (mEntityToIndex[entity] == MAX_ENTITIES) {
            return nullptr;
        }
        
        return &mComponentArray[mEntityToIndex[entity]];
    }
    
    // Get all components for iteration
    T* GetComponents() {
        return mComponentArray.data();
    }
    
    // Get current size
    size_t GetSize() const { return mSize; }

private:
    // Packed array of components (of type T)
    std::array<T, MAX_ENTITIES> mComponentArray;
    
    // Map from entity ID to array index
    std::array<Entity, MAX_ENTITIES> mEntityToIndex;
    
    // Map from array index to entity ID
    std::array<Entity, MAX_ENTITIES> mIndexToEntity;
    
    // Total size of valid entries in the array
    size_t mSize = 0;
};

} // namespace ECS
} // namespace Lite2D
