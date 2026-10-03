#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "MeshData.h"
#include "TextureData.h"
#include "MaterialData.h"
#include "ShaderData.h"

#include <variant>
#include "AssetIds.h"
#include "AssetStorage.h"

namespace Engine::Core
{
	using AssetVariant = std::variant<std::monostate, std::unique_ptr<MeshData>, std::unique_ptr<TextureData>, std::unique_ptr<MaterialData>, std::unique_ptr<ShaderData>>;


	class AssetManager
	{
	private:

		// do not remove assets from the sparse sets until its memory is no longer in use
		AssetStorage<MeshData, AssetIdMax, AssetCapacity> m_meshes{};
		AssetStorage<MaterialData, AssetIdMax, AssetCapacity> m_materials{};
		AssetStorage<ShaderData, AssetIdMax, AssetCapacity> m_shaders{};
		AssetStorage<TextureData, AssetIdMax, AssetCapacity> m_textures{};

		friend class EngineSystem;
		friend class AssetPipeline;

	public:
		std::unordered_map<std::string, std::string> m_textureFilePathToNameMap{};

		AssetManager();
		~AssetManager();

		// Retrieves a mesh by name
		void get(MeshData*& meshOut, const std::string& name);
		void get(MaterialData*& matOut, const std::string& name);
		void get(ShaderData*& shaderOut, const std::string& name);
		void get(TextureData*& texOut, const std::string& name);

		//Retrieves a mesh by its id (fast)
		MeshData* getMesh(MeshId meshId);
		MaterialData* getMaterial(MaterialId materialId);
		ShaderData* getShader(ShaderId shaderId);
		TextureData* getTexture(TextureId textureId);

		void addAsset(const std::string& name, AssetVariant&& asset);

		std::vector<ShaderData*> shaderList();
		std::vector<MaterialData*> materialList();
		std::vector<MeshData*> meshList();
		std::vector<TextureData*>textureList();

		TextureId getTextureId(const std::string& name)
		{
			return m_textures.getId(name);
		}

		ShaderId getShaderId(const std::string& name)
		{
			return m_shaders.getId(name);
		}

		MeshId getMeshId(const std::string& name)
		{
			return m_meshes.getId(name);
		}


	};

}