#include "core/ecs/EntityManager.h"
#include <core/engine/Engine.h>

void Engine::Core::EntityManager::registerEntity(size_t id, const Entity& entity)
{
	entities.emplace(id, entity);
}