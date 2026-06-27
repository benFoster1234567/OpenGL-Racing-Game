#include "core/engine/Engine.h"


void Engine::Core::EngineSystem::createAssetManager()
{
	int initialShaders = assetManager.shaderMap.size();
	int initialMeshes = assetManager.meshMap.size();
	int initialTextures = assetManager.textureMap.size();

	assetPipeline.refreshAssetManager(assetManager);

	int addedShaders = assetManager.shaderMap.size() - initialShaders;
	int addedMeshes = assetManager.meshMap.size() - initialMeshes;
	int addedTextures = assetManager.textureMap.size() - initialTextures;

	if (addedShaders == 0 && addedMeshes == 0 && addedTextures == 0)
	{
		throw std::runtime_error("Nothing was imported during asset refresh!");
	}
}

void Engine::Core::EngineSystem::fillEntityRenderList(std::vector<Entity*>& entityListOut)
{
	if (entityListOut.size() > 0)
		entityListOut.clear();

	if (renderQueue.empty())
		return;

	for (EntityId id : renderQueue)
	{
		Entity* e = nullptr;
		entityManager.getEntity(id, e);
		if (e != nullptr)
			entityListOut.push_back(e);
	}
}

void Engine::Core::EngineSystem::updateRenderQueue()
{
	
		Entity* e{ nullptr };
		entityManager.getEntity(0, e);
		if (e != nullptr)
			e->rotateDegrees(15.0f * deltaTime, glm::vec3(0.5f, 1.0f, 0.0f));


}

void Engine::Core::EngineSystem::createEntities()
{
	auto entity = entityManager.submitEntity({ .shaderName = "shader", .meshName = "bunny" }, assetManager);
	auto grid = entityManager.submitEntity({ .shaderName = "shader", .meshName = "grid" }, assetManager);
	
	Entity* g = nullptr;

	entityManager.getEntity(1, g);

	g->transform.pos = glm::translate(glm::mat4(1), glm::vec3(0, -0.5f, 0));

	renderQueue.push_back(grid);
	renderQueue.push_back(entity);


}

void Engine::Core::EngineSystem::dispatchAssets()
{
	std::vector<ShaderData*> shaderList{};
	std::vector<MeshData*> meshList{};
	
	assetManager.shaderList(shaderList);
	if (shaderList.empty()) std::cout << "shader list empty!";
	else shaderDispatcher.invoke(shaderList);

	assetManager.meshList(meshList);
	if (meshList.empty()) std::cout << "mesh list empty!";
	else meshDispatcher.invoke(meshList);

}
