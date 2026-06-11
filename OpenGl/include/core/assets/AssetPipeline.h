#pragma once

#include "core/assets/AssetManager.h"

#include <iostream>
#include <string>
#include <typeindex>
#include <stack>
#include <functional>


#include <variant>
#include <optional>

namespace Engine::Core {

	class AssetManager;
	


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
				return AssetVariant(func(path, name));
			};
		}

		void processCommand(ImportCommand cmd, AssetManager& am);
		void refreshAssetManager(AssetManager& am);

	};

}