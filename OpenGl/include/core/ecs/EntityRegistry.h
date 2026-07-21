#pragma once
#include <array>
#include "ECS.h"
#include <queue>

namespace Engine::Core::ECS
{

	class EntityRegistry
	{
	private:
		std::queue<Entity> availableEntities{};
		std::array<Signature, MAX_ENTITIES> signatures{};
		uint32_t livingEntityCount{};

	public:
		EntityRegistry()
		{
			for (Entity entity{ 0 }; entity < MAX_ENTITIES; ++entity)
			{
				availableEntities.push(entity);
			}
		}

		Entity createEntity();
		void destroyEntity(Engine::Core::ECS::Entity entity);
		void setSignature(Entity entity, Signature);
		Signature getSignature(Entity entity);
	};

}