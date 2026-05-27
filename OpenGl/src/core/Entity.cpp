#include "core/ecs/Entity.h"

Engine::Core::Entity& Engine::Core::Entity::operator=(const Engine::Core::Entity& other) {
    if (this == &other) return *this;
    controller = other.controller ? other.controller : nullptr;
    transform = other.transform;
    mesh = other.mesh;
    material = other.material;
    shader = other.shader;
    return *this;
}