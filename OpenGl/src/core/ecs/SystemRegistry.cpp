#include "core/ecs/coordinator/SystemRegistry.h"

void Engine::Core::ECS::SystemRegistry::entityDestroyed(Entity entity)
{
	for (const auto& [type, system] : m_systems)
	{
		system->m_entities.erase(entity);
	}

}

void Engine::Core::ECS::SystemRegistry::entitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (const auto& [type, system] : m_systems)
	{
		const auto& systemSignature = m_signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->m_entities.insert(entity);
		}
		else
		{
			system->m_entities.erase(entity);
		}
	}

}
