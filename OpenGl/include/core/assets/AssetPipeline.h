#pragma once

#include <iostream>
#include <string>
#include <typeindex>
#include <stack>
#include <functional>

#include "MeshData.h"
#include "TextureData.h"
#include "MaterialData.h"
#include "ShaderData.h"

#include <variant>

namespace Engine::Core {

	class AssetManager;
	
	using AssetVariant = std::variant<std::monostate, std::unique_ptr<MeshData>, std::unique_ptr<TextureData>, std::unique_ptr<MaterialData>, std::unique_ptr<ShaderData>>;


	struct ImportCommand
	{
		std::string path{};
		std::string assetName{};
		std::type_index typeId;
	};

	class AssetPipeline
	{
	private:
		std::stack<ImportCommand> queue;
		std::unordered_map<std::type_index, std::function<AssetVariant(const std::string&, const std::string&)>> import;
	public:
		AssetPipeline() = default;

		template<typename T>
		void submit(const std::string& path, const std::string& assetName)
		{
			queue.push({ .path = path, .assetName = assetName, .typeId = typeid(T) });
		}
		
		template<typename T>
		void registerImportCallback(std::function<std::unique_ptr<T>(const std::string&, const std::string&)> func)
		{
			import[typeid(T)] = [func](const std::string& path, const std::string& name) -> AssetVariant
				{
					return AssetVariant(func(path));
				}
		}

		std::optional<AssetVariant> processCommand(ImportCommand cmd) const
		{
			auto it = import.find(cmd.typeId);
			if (it != import.end()) return (*it).second(cmd.path, cmd.assetName);
			else return std::nullopt;
		}
		 
		void loadAssetManager(AssetManager& am);

	};

}