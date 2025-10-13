#pragma once

#include <cstdint>

namespace Lite2D {
namespace ECS {

// Simple entity ID type
using Entity = uint32_t;

// Constants
constexpr Entity INVALID_ENTITY = 0;
constexpr Entity MAX_ENTITIES = 10000;

// Component type ID
using ComponentType = uint8_t;
constexpr ComponentType MAX_COMPONENT_TYPES = 64;

} // namespace ECS
} // namespace Lite2D
