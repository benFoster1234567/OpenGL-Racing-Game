#include "core/ecs/EntityManager.h"
#include <core/engine/Engine.h>

void Engine::Core::EntityManager::registerEntity(size_t id, const Entity& entity)
{
	std::unique_ptr<Entity> e = std::make_unique<Entity>(entity);
	entityList.push_back(std::move(e));
	auto it = --entityList.end();
	entities[id] = it->get();
}

Engine::Core::EntityId Engine::Core::EntityManager::submitEntity(EntityCreationCommand command, AssetManager& am)
{
	static EntityId currentId = 0 ; //extremely bad I think, but I guess this ensures that every id in the engine that's created is different?
	  
	MeshData* mesh = nullptr;
	am.getMesh(mesh, command.meshName);
	
	if (mesh == nullptr)
	{
		throw std::runtime_error("No mesh found!");
	}

	ShaderData* shader = nullptr;
	am.getShader(shader, command.shaderName);

	EntityId id = currentId;

	std::unique_ptr<Entity> e = std::make_unique<Entity>(id, mesh, shader);
	entityList.push_back(std::move(e));
	auto it = --entityList.end();
	entities[id] = it->get();
	
	currentId++;

	return id;
}
