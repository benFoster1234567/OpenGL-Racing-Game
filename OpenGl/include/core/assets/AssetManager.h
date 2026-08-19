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
#include "TextureFileNameRegistry.h"
#include "AssetStorage.h"

namespace Engine::Core
{
	constexpr int AssetCapacity = 512;
	constexpr int AssetIdMax = 1024;

	using AssetVariant = std::variant<std::monostate, MeshData, TextureData, MaterialData, ShaderData>;


	struct TextureInfo
	{
		std::string filePath{};
		TextureIdx textureId{};
	};

	using MeshId = size_t;
	using MaterialId = size_t;
	using ShaderId = size_t;
	using TextureId = size_t;
	
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
		std::vector<MeshData*> meshList();
		std::vector<TextureData*>textureList();

	};

}