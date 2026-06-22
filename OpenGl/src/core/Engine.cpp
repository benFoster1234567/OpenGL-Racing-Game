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

void Engine::Core::EngineSystem::dispatchAssets()
{
	std::vector<ShaderData*> shaderList{};
	assetManager.shaderList(shaderList);
	if (shaderList.empty()) std::cout << "shader list empty!";
	else shaderDispatcher.invoke(shaderList);
	std::vector<MeshData*> meshList{};
	assetManager.meshList(meshList);
	if (meshList.empty()) std::cout << "mesh list empty!";
	else meshDispatcher.invoke(meshList);

}
