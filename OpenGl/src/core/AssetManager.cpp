#include "core/assets/AssetManager.h"
#include "core/assets/MeshData.h"
#include "core/assets/MaterialData.h"
#include "core/assets/ShaderData.h"
#include <variant>

Engine::Core::AssetManager::AssetManager()
{

}

Engine::Core::AssetManager::~AssetManager() = default;

void Engine::Core::AssetManager::get(MeshData*& meshOut, const std::string& name)
{
	if (meshes.contains(name)) meshOut = meshes.get(name);
	else
	{
		std::cout << "No mesh found!\n";
		meshOut = nullptr;
	}
}

void Engine::Core::AssetManager::get(MaterialData*& matOut, const std::string& name)
{
	if (materials.contains(name)) matOut = materials.get(name);
	else
	{
		std::cout << "No material found!\n";
		matOut = nullptr;
	}
}

void Engine::Core::AssetManager::get(ShaderData*& shaderOut, const std::string& name)
{
	if (shaders.contains(name))
	{
		std::cout << "Shader " << name << "found!\n";
		shaderOut = shaders.get(name);
	}
	else 
	{
		std::cout << "No shader found when trying to return : " << name << "\n";
		shaderOut = nullptr;
	}
}

void Engine::Core::AssetManager::get(TextureData*& texOut, const std::string& name)
{
	if (!textures.contains(name))
	{
		std::cout << "No texture found when trying to return : " << name << "\n";
		texOut = nullptr;
		return;
	}

	texOut = textures.get(name);
	std::cout << "Texture found: " << name << "\n";
}

Engine::Core::MeshData* Engine::Core::AssetManager::getMesh(MeshId meshId)
{
	return meshes.get(meshId);
}

Engine::Core::MaterialData* Engine::Core::AssetManager::getMaterial(MaterialId materialId)
{
	return materials.get(materialId);
}

Engine::Core::ShaderData* Engine::Core::AssetManager::getShader(ShaderId shaderId)
{
	return shaders.get(shaderId);
}

Engine::Core::TextureData* Engine::Core::AssetManager::getTexture(TextureId textureId)
{
	return textures.get(textureId);
}

void Engine::Core::AssetManager::addAsset(const std::string& name, AssetVariant&& asset)
{
	std::visit([this, &name](auto&& arg)
		{
			using T = std::decay_t <decltype(arg)>; //find asset type

			if constexpr (std::is_same_v < T, std::unique_ptr<MeshData>>)
			{
				meshes.add(std::move(arg), name);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<ShaderData>>)
			{
				shaders.add(std::move(arg), name);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<MaterialData>>)
			{
				materials.add(std::move(arg), name);
			}

			if constexpr (std::is_same_v<T, std::unique_ptr<TextureData>>)
			{
				textures.add(std::move(arg), name);
			}

			else if constexpr (std::is_same_v<T, std::monostate>) { /* do nothing */ }

		}, std::move(asset));
}

std::vector<Engine::Core::ShaderData*> Engine::Core::AssetManager::shaderList()
{
	return shaders.getRawPointerList();
}

std::vector<Engine::Core::MaterialData*> Engine::Core::AssetManager::materialList()
{
	return materials.getRawPointerList();
}

std::vector<Engine::Core::MeshData*> Engine::Core::AssetManager::meshList()
{
	return meshes.getRawPointerList();
}

std::vector<Engine::Core::TextureData*> Engine::Core::AssetManager::textureList()
{
	return textures.getRawPointerList();;
}





