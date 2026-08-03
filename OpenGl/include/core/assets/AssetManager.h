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
		std::unordered_map<std::string, std::unique_ptr<MeshData>> meshMap{};
		std::unordered_map<std::string, std::unique_ptr<MaterialData>> materialMap{};
		std::unordered_map<std::string, std::unique_ptr<ShaderData>> shaderMap{};


		friend class EngineSystem;
		friend class AssetPipeline;
	public:
	
		AssetManager();
		~AssetManager();

		TextureFileNameRegistry textureFileNameRegistry{};

		// Retrieves a mesh by name
		void getMesh(MeshData*& meshOut, const std::string& name);
		void getMaterial(MaterialData*& matOut, const std::string& name);
		void getShader(ShaderData*& shaderOut, const std::string& name);
		//texture retrieval is handled by an external library in the infrastructure layer, so we don't store textures in the asset manager. The texture manager handles texture indices which represent the textures. These are sent to the render pipeline to indicate which textures to use for rendering.
		//void getTexture(TextureData*& texOut, const std::string& name);
		void getTexture(TextureIdx& texOut, const std::string& name);

		void addAsset(const std::string& name, AssetVariant&& asset);

		void shaderList(std::vector<ShaderData*>& shadersOut);
		void meshList(std::vector<MeshData*>& meshesOut);
		void textureList(std::vector<TextureInfo>& texturesOut);

	};

}