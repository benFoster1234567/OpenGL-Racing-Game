#include "core/ECS/EntityRegistry.h"
#include <assert.h>

Engine::Core::ECS::Entity Engine::Core::ECS::EntityRegistry::createEntity()
{
    assert(livingEntityCount < MAX_ENTITIES && "Too many entities!");
    auto entity = availableEntities.front();
    availableEntities.pop();
    livingEntityCount++;
    return entity;
}

void Engine::Core::ECS::EntityRegistry::destroyEntity(Engine::Core::ECS::Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    signatures[entity].reset();
    availableEntities.push(entity);
    --livingEntityCount;
}

void Engine::Core::ECS::EntityRegistry::setSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range!");
    signatures[entity] = signature;
}

Engine::Core::ECS::Signature Engine::Core::ECS::EntityRegistry::getSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range!");
    return signatures[entity];
}
