#include "core/engine/Engine.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>


void Engine::Core::EngineSystem::publishAssets()
{
	std::vector<ShaderData*> shaderList{};
	assetManager.shaderList(shaderList);
	std::vector<MeshData*> meshList{};
	assetManager.meshList(meshList);
	shaderDispatcher.invoke(shaderList);
	meshDispatcher.invoke(meshList);
}

void Engine::Core::EngineSystem::updateMouse(double xpos, double ypos)
{
	inputHandler.updateMousePosition({ xpos,ypos });
}
void Engine::Core::EngineSystem::zeroMouse()
{
	inputHandler.mouseState.zeroMouseDelta();
}

void Engine::Core::EngineSystem::updateInputState()
{
	//std::cout << "updating input state\n";
	inputHandler.updateKeyboard();
}

//loads assets from files
void Engine::Core::EngineSystem::createAssetManager()
{
	int initialShaders = assetManager.shaderMap.size();
	int initialMeshes = assetManager.meshMap.size();
	int initialTextures = assetManager.textureMap.size();

	assetPipeline.populateAssetManager(assetManager);

	int addedShaders = assetManager.shaderMap.size() - initialShaders;
	int addedMeshes = assetManager.meshMap.size() - initialMeshes;
	int addedTextures = assetManager.textureMap.size() - initialTextures;

	if (addedShaders == 0 && addedMeshes == 0 && addedTextures == 0)
	{
		throw std::runtime_error("Nothing was imported during asset refresh!");
	}
}


