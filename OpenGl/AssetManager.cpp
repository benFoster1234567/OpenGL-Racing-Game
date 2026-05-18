#include "AssetManager.h"
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

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

std::optional<Material*> 
AssetManager::getMaterial(const std::string& name) const
{
	auto it = materialMap.find(name);
	if (it != materialMap.end())
	{
		return std::optional<Material*>(it->second.get());
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

std::optional<Texture*> 
AssetManager::getTexture(const std::string& name) const
{
	auto it = textureMap.find(name);
	if (it != textureMap.end())
	{
		return std::optional<Texture*>(it->second.get());
	}
	return std::nullopt;
}

void 
AssetManager::addMesh(const std::string& name, std::unique_ptr<MeshData> mesh)
{
	meshMap[name] = std::move(mesh);
}

void 
AssetManager::addMaterial(const std::string& name, std::unique_ptr<Material> material)
{
	materialMap[name] = std::move(material);
}

void
AssetManager::addShader(const std::string& name, std::unique_ptr<Shader> shader)
{
	shaderMap[name] = std::move(shader);
}

void 
AssetManager::addTexture(const std::string& name, std::unique_ptr<Texture> texture)
{
	textureMap[name] = std::move(texture);
}

