#include "core/ecs/EntityManager.h"
#include <core/engine/Engine.h>

//enter entity into our entitymanager. Ensure entity manager stays in scope so we don't get dangling pointers. (maybe its okay to make copies of entities, but for now we have this...)
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

void Engine::Core::EntityManager::setEntityTransform(EntityId id, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scale)
{
	if (!entities.contains(id)) return;

	Entity* e = entities[id];
	e->transform.pos = translation;
	e->transform.rot = rotation;
	e->transform.scale = scale;

	e->transformMatrix = translation * rotation * scale;

}

//TODO: get rid of this...
void Engine::Core::EntityManager::setEntityTransform(EntityId id, glm::mat4 newTransform)
{
	if (!entities.contains(id)) return;
	entities[id]->transformMatrix = newTransform;

}
