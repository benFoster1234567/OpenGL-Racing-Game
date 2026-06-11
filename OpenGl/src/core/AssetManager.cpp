#include "core/assets/AssetManager.h"
#include "core/assets/MeshData.h"
#include "core/assets/MaterialData.h"
#include "core/assets/ShaderData.h"
#include <variant>

using namespace Engine::Core;

Engine::Core::AssetManager::~AssetManager() = default;

std::optional<MeshData*>
AssetManager::getMesh(const std::string& name) const
{
	auto it = meshMap.find(name);
	if (it != meshMap.end())
	{
		return std::optional<MeshData*>(it->second.get());
	}
	return std::nullopt;
}

std::optional<MaterialData*> 
AssetManager::getMaterial(const std::string& name) const
{
	auto it = materialMap.find(name);
	if (it != materialMap.end())
	{
		return std::optional<MaterialData*>(it->second.get());
	}
	return std::nullopt;
}

std::optional<ShaderData*>
AssetManager::getShader(const std::string& name) const
{
	auto it = shaderMap.find(name);
	if (it != shaderMap.end())
	{
		return std::optional<ShaderData*>(it->second.get());
	}
	return std::nullopt;
}

std::optional<TextureData*> 
AssetManager::getTexture(const std::string& name) const
{
	auto it = textureMap.find(name);
	if (it != textureMap.end())
	{
		return std::optional<TextureData*>(it->second.get());
	}
	return std::nullopt;
}




