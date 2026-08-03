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

	struct TextureImportCommand
	{
		std::string path{};
	};

	class AssetPipeline
	{
	private:
		std::stack<ImportCommand> queue;
		std::stack<TextureImportCommand> textureQueue;
		std::unordered_map<std::type_index, std::function<AssetVariant(const std::string&, const std::string&)>> import;
	public:
		AssetPipeline() = default;

		template<typename T>
		void submit(const std::string& path, const std::string& assetName)
		{
			std::type_index ti = typeid(T);
			assert(import.contains(ti) && "no import func found for type");
			queue.push({ .path = path, .assetName = assetName, .typeId = typeid(T) });
		}
		
		template<>
		void submit<TextureData>(const std::string& path, const std::string& assetName)
		{
			textureQueue.push({ .path = path });
		}

		template<typename T>
		void registerImportCallback(std::function<std::unique_ptr<T>(const std::string&, const std::string&)> func)
		{
			import[typeid(T)] = [func](const std::string& path, const std::string& name) -> AssetVariant
			{
				return AssetVariant(func(path, name));
			};
		}

		void processCommand(const ImportCommand& cmd, AssetManager& am)
		{
			auto it = import.find(cmd.typeId);
			if (it == import.end()) return;
			am.addAsset(cmd.assetName, it->second(cmd.path, cmd.assetName));
		}

		void populateAssetManager(AssetManager& am)
		{
			while (!textureQueue.empty())
			{
				TextureImportCommand tcmd = textureQueue.top();
				textureQueue.pop();
				am.textureFileNameRegistry.addNewTexture(tcmd.path);
			}

			while (!queue.empty())
			{
				Engine::Core::ImportCommand icmd = queue.top();
				queue.pop();
				processCommand(icmd, am);
			}

		}

	};

}