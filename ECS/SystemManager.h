#pragma once

#include "System.h"
#include "EntityManager.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>

namespace Lite2D {
namespace ECS {

/**
 * System Manager
 * Handles system registration, execution order, and lifecycle
 */
class SystemManager {
public:
    SystemManager() = default;
    ~SystemManager() = default;
    
    // Delete copy constructor and assignment operator
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    
    // System registration and management
    template<typename T, typename... Args>
    std::shared_ptr<T> RegisterSystem(Args&&... args);
    
    template<typename T>
    void SetSystemSignature(std::bitset<MAX_COMPONENT_TYPES> signature);
    
    template<typename T>
    std::shared_ptr<T> GetSystem();
    
    // System execution
    void UpdateSystems(EntityManager& entityManager, float deltaTime);
    
    // Entity lifecycle management
    void OnEntityDestroyed(Entity entity);
    void OnEntitySignatureChanged(Entity entity, std::bitset<MAX_COMPONENT_TYPES> signature);
    
    // System management
    void InitializeAllSystems(EntityManager& entityManager);
    void ShutdownAllSystems(EntityManager& entityManager);
    
    // Utility
    size_t GetSystemCount() const { return mSystems.size(); }
    void EnableSystem(const std::string& systemName, bool enabled);
    
    // Debug
    void PrintSystemInfo() const;

private:
    // Map from system type to system instance
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems;
    
    // Map from system type to required component signature
    std::unordered_map<std::type_index, std::bitset<MAX_COMPONENT_TYPES>> mSignatures;
    
    // Systems in execution order
    std::vector<std::shared_ptr<System>> mSystemsToUpdate;
    
    // Helper function to get system type index
    template<typename T>
    std::type_index GetSystemTypeIndex();
};

// Template implementations
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
