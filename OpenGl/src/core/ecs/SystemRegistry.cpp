#include "core/ECS/SystemRegistry.h"

void Engine::Core::ECS::SystemRegistry::entityDestroyed(Entity entity)
{
	for (const auto& [type, system] : systems)
	{
		system->entities.erase(entity);
	}

}

void Engine::Core::ECS::SystemRegistry::entitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (const auto& [type, system] : systems)
	{
		const auto& systemSignature = signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->entities.insert(entity);
		}
		else
		{
			system->entities.erase(entity);
		}
	}

}
