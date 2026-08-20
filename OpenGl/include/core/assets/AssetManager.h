#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

#include "MeshData.h"
#include "TextureData.h"
#include "MaterialData.h"
#include "ShaderData.h"


#include "core/Patterns.h"
#include <utility>
#include <variant>
#include <algorithm>
#include "AssetIds.h"
#include "TextureFileNameRegistry.h"
#include "AssetStorage.h"

namespace Engine::Core
{
	using AssetVariant = std::variant<std::monostate, std::unique_ptr<MeshData>, std::unique_ptr<TextureData>, std::unique_ptr<MaterialData>, std::unique_ptr<ShaderData>>;


	struct TextureInfo
	{
		std::string filePath{};
		TextureIdx textureId{};
	};
	
	class AssetManager
	{
	private:

		// do not remove assets from the sparse sets until its memory is no longer in use
		AssetStorage<MeshData, AssetIdMax, AssetCapacity> meshes{};
		AssetStorage<MaterialData, AssetIdMax, AssetCapacity> materials{};
		AssetStorage<ShaderData, AssetIdMax, AssetCapacity> shaders{};
		AssetStorage<TextureData, AssetIdMax, AssetCapacity> textures{};

		friend class EngineSystem;
		friend class AssetPipeline;

	public:
		std::unordered_map<std::string, std::string> textureFilePathToNameMap{};

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
			return textures.getId(name);
		}

		ShaderId getShaderId(const std::string& name)
		{
			return shaders.getId(name);
		}

		MeshId getMeshId(const std::string& name)
		{
			return meshes.getId(name);
		}


	};

}