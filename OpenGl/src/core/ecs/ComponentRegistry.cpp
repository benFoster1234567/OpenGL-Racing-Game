#include "core/ecs/ComponentRegistry.h"

void Engine::Core::ECS::ComponentRegistry::entityDestroyed(Entity entity)
{
	for (const auto& [type, component] : componentArrays)
	{
		component->entityDestroyed(entity);
	}
}
