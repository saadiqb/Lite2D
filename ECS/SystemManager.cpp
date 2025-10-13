#include "SystemManager.h"
#include <iostream>
#include <algorithm>

namespace Lite2D {
namespace ECS {

void SystemManager::UpdateSystems(EntityManager& entityManager, float deltaTime) {
    for (auto& system : mSystemsToUpdate) {
        if (system->IsEnabled()) {
            system->Update(entityManager, deltaTime);
        }
    }
}

void SystemManager::OnEntityDestroyed(Entity entity) {
    // Systems don't need to handle entity destruction directly
    // The EntityManager handles component cleanup automatically
    // Systems will simply not find the entity in their queries
}

void SystemManager::OnEntitySignatureChanged(Entity entity, std::bitset<MAX_COMPONENT_TYPES> signature) {
    // Check each system to see if the entity should be added or removed
    for (auto const& pair : mSystems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSignatures[type];
        
        // Check if entity signature matches system requirements
        bool entityMatchesSystem = (signature & systemSignature) == systemSignature;
        
        // Note: In a more complex system, we might want to notify systems
        // about entities being added/removed from their consideration
        // For now, systems will handle this during their Update() calls
    }
}

void SystemManager::InitializeAllSystems(EntityManager& entityManager) {
    for (auto& system : mSystemsToUpdate) {
        system->Initialize(entityManager);
    }
}

void SystemManager::ShutdownAllSystems(EntityManager& entityManager) {
    for (auto& system : mSystemsToUpdate) {
        system->Shutdown(entityManager);
    }
}

void SystemManager::EnableSystem(const std::string& systemName, bool enabled) {
    for (auto& system : mSystemsToUpdate) {
        if (std::string(system->GetName()) == systemName) {
            system->SetEnabled(enabled);
            break;
        }
    }
}

void SystemManager::PrintSystemInfo() const {
    std::cout << "=== System Manager Info ===" << std::endl;
    std::cout << "Registered Systems: " << mSystems.size() << std::endl;
    
    for (const auto& system : mSystemsToUpdate) {
        std::cout << "- " << system->GetName() 
                  << " (enabled: " << (system->IsEnabled() ? "yes" : "no") << ")" 
                  << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

} // namespace ECS
} // namespace Lite2D
