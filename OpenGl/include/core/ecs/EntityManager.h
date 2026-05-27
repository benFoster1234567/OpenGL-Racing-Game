#pragma once
#include "core/events/EventDispatcher.h"
#include "core/ecs/Entity.h"
#include <functional>
namespace Engine::Core
{
	class EntityManager
	{
	public:
		void registerEntity(size_t id, const Entity& entity);
		void bindRenderSubmitCallback(std::function<void(Entity)> func);
		EntityId createAndRegisterEntity(MeshData* mesh, MaterialData* material);
		void addToRenderQueue(EntityId id);
		void invokeRenderSubmitCallbacks(Entity entity);
	private:
		std::unordered_map<EntityId, Entity> entities;
		std::vector<EntityId> renderQueue;
		EventDispatcher<Entity> submitToRenderer;

	};
}