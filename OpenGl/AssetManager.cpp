#include "AssetManager.h"

std::optional<std::shared_ptr<Mesh>> 
AssetManager::getMesh(const std::string& name) const
{
	auto it = meshMap.find(name);
	if (it != meshMap.end())
	{
		return std::optional<std::shared_ptr<Mesh>>(it->second);
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<Material>> 
AssetManager::getMaterial(const std::string& name) const
{
	auto it = materialMap.find(name);
	if (it != materialMap.end())
	{
		return std::optional<std::shared_ptr<Material>>(it->second);
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<Shader>> 
AssetManager::getShader(const std::string& name) const
{
	auto it = shaderMap.find(name);
	if (it != shaderMap.end())
	{
		return std::optional<std::shared_ptr<Shader>>(it->second);
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<Texture>> 
AssetManager::getTexture(const std::string& name) const
{
	auto it = textureMap.find(name);
	if (it != textureMap.end())
	{
		return std::optional<std::shared_ptr<Texture>>(it->second);
	}
	return std::nullopt;
}

void 
AssetManager::addMesh(const std::string& name, std::shared_ptr<Mesh> mesh)
{
	meshMap[name] = mesh;
}

void 
AssetManager::addMaterial(const std::string& name, std::shared_ptr<Material> material)
{
	textureMap[material->texture->GetName()] = material->texture;
	if (material->normalMap)
	{
		textureMap[material->normalMap->GetName()] = material->normalMap;
	}
	materialMap[name] = material;
}

void 
AssetManager::addShader(const std::string& name, std::shared_ptr<Shader> shader)
{
	shaderMap[name] = shader;
}

void 
AssetManager::addTexture(const std::string& name, std::shared_ptr<Texture> texture)
{
	textureMap[name] = texture;
}

