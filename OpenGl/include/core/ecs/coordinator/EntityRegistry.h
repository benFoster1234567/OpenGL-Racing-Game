#pragma once
#include <array>
#include "ECS.h"
#include <queue>

namespace Engine::Core::ECS
{

	class EntityRegistry
	{
	private:
		std::queue<Entity> m_availableEntities{};
		std::array<Signature, MAX_ENTITIES> m_signatures{};
		uint32_t m_livingEntityCount{};

	public:
		EntityRegistry()
		{
			for (Entity entity{ 0 }; entity < MAX_ENTITIES; ++entity)
			{
				m_availableEntities.push(entity);
			}
		}

		Entity createEntity();
		void destroyEntity(Engine::Core::ECS::Entity entity);
		void setSignature(Entity entity, Signature);
		Signature getSignature(Entity entity);
	};

}