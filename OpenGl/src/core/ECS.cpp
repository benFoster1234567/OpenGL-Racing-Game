#include "core/ecs/ECS.h"



Engine::Core::ECS::System Engine::Core::ECS::SystemRegistry::registerSystem(SystemFunction systemFunc)
{
	auto currentSystem = nextSystem;
	systemMap[currentSystem] = systemFunc;
	nextSystem++;
	return currentSystem;
}

Engine::Core::ECS::Entity Engine::Core::ECS::EntityComponentSystemManager::createEntity()
{
	Entity e = nextEntityId++;
	entities.push_back(e);
	return e;
}

void Engine::Core::ECS::EntityComponentSystemManager::attachSystem(Entity entity, System system)
{
	entitySystemMap[entity].push_back(int(system));
}

void Engine::Core::ECS::EntityComponentSystemManager::updateSystems()
{
	for (auto& [entity, systemList] : entitySystemMap)
	{
		for (auto s : systemList)
		{
			systems.systemMap[s](entity, components);
		}
	}
}

Engine::Core::ECS::ComponentBase::~ComponentBase() = default;

Engine::Core::ECS::IComponentArray::~IComponentArray() = default;

void Engine::Core::ECS::CameraComponent::rotate(float degrees, glm::vec3 axis)
{
	view = glm::rotate(view, glm::radians(degrees), axis);
}
