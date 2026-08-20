#include "core/engine/Engine.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>


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

void Engine::Core::EngineSystem::setTextureUvTiling(std::string textureName, glm::vec2 uvTiling)
{
	TextureData* texture = nullptr;
	assetManager.get(texture, textureName);
	texture->uvTiling = uvTiling;
}

void Engine::Core::EngineSystem::fillStaticLightVector(std::vector<ECS::StaticPointLightRendererData>& lights)
{
	game.setupLights(lights);
}

//loads assets from files
void Engine::Core::EngineSystem::createAssetManager()
{
	size_t initialShaders = assetManager.shaders.size();
	size_t initialMeshes = assetManager.meshes.size();

	assetPipeline.populateAssetManager(assetManager);

	size_t addedShaders = assetManager.shaders.size() - initialShaders;
	size_t addedMeshes = assetManager.meshes.size() - initialMeshes;

	if (addedShaders == 0 && addedMeshes == 0)
	{
		throw std::runtime_error("Nothing was imported during asset refresh!");
	}
}


