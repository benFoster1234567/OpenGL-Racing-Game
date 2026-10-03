#include "core/engine/Engine.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>


void Engine::Core::EngineSystem::updateMouse(double xpos, double ypos)
{
	m_inputHandler.updateMousePosition({ xpos,ypos });
}
void Engine::Core::EngineSystem::zeroMouse()
{
	m_inputHandler.m_mouseState.zeroMouseDelta();
}

void Engine::Core::EngineSystem::pollPhysicsEvents(const std::vector<ECS::PhysicsEvent>& eventQueue)
{
	m_game.pollPhysicsEvents(eventQueue);
}

void Engine::Core::EngineSystem::updateInputState()
{
	//std::cout << "updating input state\n";
	m_inputHandler.updateKeyboard();
}

void Engine::Core::EngineSystem::setTextureUvTiling(std::string textureName, glm::vec2 uvTiling)
{
	TextureData* texture = nullptr;
	m_assetManager.get(texture, textureName);
	texture->uvTiling = uvTiling;
}

void Engine::Core::EngineSystem::fillStaticLightVector(std::vector<ECS::StaticPointLightRendererData>& lights)
{
	m_game.setupLights(lights);
}

//loads assets from files
void Engine::Core::EngineSystem::createAssetManager()
{
	size_t initialShaders = m_assetManager.m_shaders.size();
	size_t initialMeshes = m_assetManager.m_meshes.size();

	m_assetPipeline.populateAssetManager(m_assetManager);

	size_t addedShaders = m_assetManager.m_shaders.size() - initialShaders;
	size_t addedMeshes = m_assetManager.m_meshes.size() - initialMeshes;

	if (addedShaders == 0 && addedMeshes == 0)
	{
		throw std::runtime_error("Nothing was imported during asset refresh!");
	}
}


