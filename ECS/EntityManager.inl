#pragma once

#include "EntityManager.h"
#include "ComponentArray.h"
#include <algorithm>
#include <vector>
#include <iostream>

namespace Lite2D {
namespace ECS {

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
    
    // Create a signature that matches all the required components
    std::bitset<MAX_COMPONENT_TYPES> requiredSignature;
    (requiredSignature.set(GetComponentType<Components>()), ...);
    
    // Check each entity's signature
    for (Entity entity = 1; entity <= MAX_ENTITIES; ++entity) {
        if (!IsValid(entity)) continue;
        
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
