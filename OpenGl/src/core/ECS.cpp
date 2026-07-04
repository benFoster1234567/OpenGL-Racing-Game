#include "core/ecs/ECS.h"

void Engine::Core::ECS::SystemRegistry::update(Entity entity, ComponentRegistry& compreg)
{
	if (!systemMap.contains(entity))
	{
		return;
	}

	for (const auto& s : systemMap[entity])
	{
		s(entity, compreg);
	}

}

void Engine::Core::ECS::SystemRegistry::registerSystem(Entity entity, SystemFunction systemFunc)
{
	if (!systemMap.contains(entity))
	{
		systemMap[entity] = std::vector<SystemFunction>{};
	}

	systemMap[entity].push_back(systemFunc);
}

Engine::Core::ECS::Entity Engine::Core::ECS::EntityComponentSystemManager::createEntity()
{
	Entity e = nextEntityId++;
	entities.push_back(e);
	return e;
}

void Engine::Core::ECS::EntityComponentSystemManager::addSystem(Entity entity, SystemRegistry::SystemFunction systemFunc)
{
	systems.registerSystem(entity, systemFunc);
}

void Engine::Core::ECS::EntityComponentSystemManager::updateSystems()
{
	for (const auto& entity : entities)
	{
		systems.update(entity, components);
	}

}

Engine::Core::ECS::IComponent::~IComponent() = default;

Engine::Core::ECS::IComponentArray::~IComponentArray() = default;
