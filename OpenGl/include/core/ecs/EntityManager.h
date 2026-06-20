#pragma once
#include "core/events/EventDispatcher.h"
#include "core/ecs/Entity.h"
#include <functional>
#include"core/assets/AssetManager.h"
namespace Engine::Core
{
	struct EntityCreationCommand
	{
		std::string shaderName;
		std::string meshName;
	};

	class EntityManager
	{
	public:
		EntityManager() = default;
		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = default;
		EntityManager& operator=(const EntityManager&) = delete;
		
		void registerEntity(size_t id, const Entity& entity);
		
		EntityId submitEntity(EntityCreationCommand command, AssetManager& am);

	private:
		std::unordered_map<EntityId, Entity*> entities{};
		std::vector<std::unique_ptr<Entity>> entityList{};
		
	};

}