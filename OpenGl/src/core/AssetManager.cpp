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

void Engine::Core::AssetManager::getMaterial(MaterialData*& matOut, const std::string& name)
{
	if (materialMap.contains(name)) matOut = materialMap[name].get();
	else
	{
		std::cout << "Nom material found!\n";
		matOut = nullptr;
	}
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

void Engine::Core::AssetManager::getTexture(TextureData*& texOut, const std::string& name)
{
	if (!textureMap.contains(name))
	{
		std::cout << "No texture found when trying to return : " << name << "\n";
		texOut = nullptr;
		return;
	}

	texOut = textureMap[name].get();
	std::cout << "Texture found: " << name << "\n";
}

void Engine::Core::AssetManager::addAsset(const std::string& name, AssetVariant&& asset)
{
	std::visit([this, &name](auto&& arg)
		{
			using T = std::decay_t <decltype(arg)>; //find asset type

			if constexpr (std::is_same_v < T, std::unique_ptr<MeshData>>)
			{
				meshMap[name] = std::move(arg);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<ShaderData>>)
			{
				shaderMap[name] = std::move(arg);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<MaterialData>>)
			{
				materialMap[name] = std::move(arg);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<TextureData>>)
			{
				textureMap[name] = std::move(arg);
			}

		}, std::move(asset));
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

void Engine::Core::AssetManager::textureList(std::vector<TextureData*>& texturesOut)
{
	for (const auto& [name, texture] : textureMap)
	{
		texturesOut.push_back(texture.get());
	}
}




