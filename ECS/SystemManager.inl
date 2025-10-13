#pragma once

#include "SystemManager.h"

namespace Lite2D {
namespace ECS {

template<typename T, typename... Args>
std::shared_ptr<T> SystemManager::RegisterSystem(Args&&... args) {
    const char* typeName = typeid(T).name();
    std::type_index typeIndex = std::type_index(typeid(T));
    
    // Check if system already exists
    auto it = mSystems.find(typeIndex);
    if (it != mSystems.end()) {
        return std::static_pointer_cast<T>(it->second);
    }
    
    // Create system instance
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    mSystems.insert({typeIndex, system});
    mSystemsToUpdate.push_back(system);
    
    return system;
}

template<typename T>
void SystemManager::SetSystemSignature(std::bitset<MAX_COMPONENT_TYPES> signature) {
    std::type_index typeIndex = std::type_index(typeid(T));
    mSignatures.insert({typeIndex, signature});
}

template<typename T>
std::shared_ptr<T> SystemManager::GetSystem() {
    std::type_index typeIndex = std::type_index(typeid(T));
    auto it = mSystems.find(typeIndex);
    if (it != mSystems.end()) {
        return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template<typename T>
std::type_index SystemManager::GetSystemTypeIndex() {
    return std::type_index(typeid(T));
}

} // namespace ECS
} // namespace Lite2D
