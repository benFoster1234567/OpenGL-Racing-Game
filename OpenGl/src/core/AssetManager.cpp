#include "core/assets/AssetManager.h"

using namespace Engine::Core;

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

std::optional<Shader*> 
AssetManager::getShader(const std::string& name) const
{
	auto it = shaderMap.find(name);
	if (it != shaderMap.end())
	{
		return std::optional<Shader*>(it->second.get());
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

void 
AssetManager::addMesh(const std::string& name, std::unique_ptr<MeshData> mesh)
{
	meshMap[name] = std::move(mesh);
}

void 
AssetManager::addMaterial(const std::string& name, std::unique_ptr<MaterialData> material)
{
	materialMap[name] = std::move(material);
}

void
AssetManager::addShader(const std::string& name, std::unique_ptr<Shader> shader)
{
	shaderMap[name] = std::move(shader);
}

void 
AssetManager::addTexture(const std::string& name, std::unique_ptr<TextureData> texture)
{
	textureMap[name] = std::move(texture);
}

