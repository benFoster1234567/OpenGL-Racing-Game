#include "core/assets/AssetManager.h"
#include "core/assets/MeshData.h"
#include "core/assets/MaterialData.h"
#include "core/assets/ShaderData.h"
#include <variant>



Engine::Core::AssetManager::AssetManager()
{
	meshMap["grid"] = std::make_unique<GridData>();
}

Engine::Core::AssetManager::~AssetManager() = default;

void Engine::Core::AssetManager::getMesh(MeshData*& meshOut, const std::string& name)
{
	if (meshMap.contains(name)) meshOut = meshMap[name].get();
	else
	{
		std::cout << "Nom mesh found!\n";
		meshOut = nullptr;
	}
}

void Engine::Core::AssetManager::getMaterial(MaterialData* matOut, const std::string& name)
{
	if (materialMap.contains(name)) matOut = materialMap[name].get();
	else matOut = nullptr;
}

void Engine::Core::AssetManager::getShader(ShaderData*& shaderOut, const std::string& name)
{
	if (shaderMap.contains(name))
	{
		std::cout << "Shader " << name << "found!\n";
		shaderOut = shaderMap[name].get();
	}
	else 
	{
		std::cout << "No shader found when trying to return : " << name << "\n";
		shaderOut = nullptr;
	}
}

void Engine::Core::AssetManager::getTexture(TextureData* texOut, const std::string& name)
{
	if (textureMap.contains(name)) texOut = textureMap[name].get();
	else texOut = nullptr;
}

void Engine::Core::AssetManager::shaderList(std::vector<ShaderData*>& shadersOut)
{
	for (const auto& s : shaderMap)
	{
		ShaderData* shader = s.second.get();

		if (shader != NULL)
		{
			shadersOut.push_back(shader);
		}
	}
}

void Engine::Core::AssetManager::meshList(std::vector<MeshData*>& meshesOut)
{
	for (auto& [key, val] : meshMap)
	{
		meshesOut.push_back(val.get());
	}
}




