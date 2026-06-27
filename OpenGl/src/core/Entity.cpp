#include "core/ecs/Entity.h"

Engine::Core::Entity& Engine::Core::Entity::operator=(const Engine::Core::Entity& other) {
    if (this == &other) return *this;
    controller = other.controller ? other.controller : nullptr;
    transform = other.transform;
    transformMatrix = other.transformMatrix;
    mesh = other.mesh;
    return *this;
}

void Engine::Core::Entity::rotateRadians(float radians, glm::vec3 axis)
{
    transform.rot = glm::rotate(transform.rot, radians, axis);
}
void Engine::Core::Entity::rotateDegrees(float degrees, glm::vec3 axis)
{
    transform.rot = glm::rotate(transform.rot, glm::radians(degrees), axis);
}
