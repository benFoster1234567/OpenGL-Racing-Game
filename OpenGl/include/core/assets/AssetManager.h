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


namespace Engine::Core 
{
	using AssetVariant = std::variant<std::monostate, std::unique_ptr<MeshData>, std::unique_ptr<TextureData>, std::unique_ptr<MaterialData>, std::unique_ptr<ShaderData>>;


	class AssetManager : public Patterns::Singleton<AssetManager>
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<MeshData>> meshMap{};
		std::unordered_map<std::string, std::unique_ptr<MaterialData>> materialMap{};
		std::unordered_map<std::string, std::unique_ptr<TextureData>> textureMap{};
		std::unordered_map<std::string, std::unique_ptr<ShaderData>> shaderMap{};

		friend class AssetPipeline;

	public:
		AssetManager() = default;
		~AssetManager();

		// Retrieves a mesh by name. Returns nullopt if the mesh is not found
		std::optional<MeshData*> getMesh(const std::string& name) const;
		std::optional<MaterialData*> getMaterial(const std::string& name) const;
		std::optional<ShaderData*> getShader(const std::string& name) const;
		std::optional<TextureData*> getTexture(const std::string& name) const;

		void addAsset(const std::string& name, AssetVariant&& asset)
		{
			std::visit([this, &name](auto&& arg)
			{
				using T = std::decay_t <decltype(arg)>; //find asset type

				if constexpr(std::is_same_v < T, std::unique_ptr<MeshData>>)
				{
					meshMap[name] = std::move(arg);
				}

				if constexpr (std::is_same_v<T, std::unique_ptr<ShaderData>>)
				{
					shaderMap[name] = std::move(arg);
				}

			}, std::move(asset));


		}
		


	};

}