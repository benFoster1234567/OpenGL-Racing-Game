#include "core/ecs/coordinator/EntityRegistry.h"
#include <assert.h>

Engine::Core::ECS::Entity Engine::Core::ECS::EntityRegistry::createEntity()
{
	assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities!");
	auto entity = m_availableEntities.front();
	m_availableEntities.pop();
	m_livingEntityCount++;
	return entity;
}

void Engine::Core::ECS::EntityRegistry::destroyEntity(Engine::Core::ECS::Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	m_signatures[entity].reset();
	m_availableEntities.push(entity);
	--m_livingEntityCount;
}

void Engine::Core::ECS::EntityRegistry::setSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES && "Entity out of range!");
	m_signatures[entity] = signature;
}

Engine::Core::ECS::Signature Engine::Core::ECS::EntityRegistry::getSignature(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range!");
	return m_signatures[entity];
}
