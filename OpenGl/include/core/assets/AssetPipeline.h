#pragma once

#include "core/assets/AssetManager.h"

#include <iostream>
#include <string>
#include <typeindex>
#include <stack>
#include <functional>


#include <variant>
#include <optional>
#include <chrono>

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
		std::unordered_map<std::type_index, std::function<AssetVariant(const std::string&, const std::string&)>> import;

		static void createSolidColorTexture(TextureData*& texOut, glm::vec3 color)
		{
			texOut->width = 1;
			texOut->height = 1;
			texOut->channels = 4;
			auto& pixelData = texOut->pixels;
			pixelData.clear();
			pixelData.push_back(color.x * 255);
			pixelData.push_back(color.y * 255);
			pixelData.push_back(color.z * 255);
		}

		static std::string addSolidColorTexMap( MaterialData* material
			, MaterialData::MapType type
			, AssetManager& assetManager )
		{
			std::string textureName = material->name + "_" + std::to_string(int(type));
			glm::vec3 color = material->getMatColor(type);
			std::unique_ptr<TextureData> newTexture = std::make_unique<TextureData>();
			TextureData* newTexPtr = newTexture.get();
			createSolidColorTexture(newTexPtr, color);
			assetManager.addAsset(textureName, std::move(newTexture));
			TextureData* td = nullptr;
			assetManager.get(td, textureName);

			assert(td != nullptr);

			return textureName;
		}

	public:
		AssetPipeline() = default;

		const std::string assetFilePath = "assets/";

		template<typename T>
		void submit(const std::string& path, const std::string& assetName)
		{
			std::type_index ti = typeid(T);
			assert(import.contains(ti) && "no import func found for type");
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

		bool processCommand(const ImportCommand& cmd, AssetManager& am)
		{
			auto it = import.find(cmd.typeId);
			
			if (it == import.end())
			{
				std::cerr << "No import function found for type. " << cmd.assetName << " not imported.\n";
				return false;
			}

			auto asset = it->second(cmd.path, cmd.assetName);

			bool assetNull = std::visit([](const auto& ptr) {
				using T = std::decay_t<decltype(ptr)>;
				if constexpr (std::is_same_v<T, std::monostate>) return true;
				else return ptr == nullptr; 
			}, asset);

			if (assetNull)
			{
				std::cerr << "Asset import failed: " << cmd.assetName << " | " << cmd.path << "\n";
				return false;
			}

			else
			{ 
				am.addAsset(cmd.assetName, std::move(asset));
				std::cout << "Asset successfully imported: " << cmd.assetName << " | " << cmd.path << "\n";
				return true;
			}

		}

		void populateAssetManager(AssetManager& am)
		{
			std::cout << "Populating asset manager\n";
			std::type_index textureType = typeid(TextureData);

			auto start = std::chrono::high_resolution_clock::now();

			while (!queue.empty())
			{
				Engine::Core::ImportCommand icmd = queue.top();
				queue.pop();

				bool imported = processCommand(icmd, am);

			}

			//filling materials with texture pointers
			for (auto& [matName, mat] : am.materialMap)
			{
				for (int i{ 0 }; i < MaterialData::MAX_MAPTYPES; i++)
				{
					std::string partialPath = mat.get()->mapFilePaths[i];
					std::string textureName;
					MaterialData::MapType mapType = MaterialData::MapType(i);

					if (partialPath == "" || partialPath.empty())
					{
						std::cout << "No map found, creating new texture";
						textureName = addSolidColorTexMap(mat.get(), mapType, am);
					}
					
					else
					{
						std::string filePath = "assets/materials/" + partialPath;
						std::cout << mat.get()->name << ": file path for map " << i << ": " << filePath << "\n";

						if (!am.textureFilePathToNameMap.contains(filePath))
						{
							std::cerr << "Texture name not found for filepath: " << filePath << "\n";
							continue;
						}

						textureName = am.textureFilePathToNameMap[filePath];
					}

					if (!am.textureMap.contains(textureName))
					{
						std::cerr << "Texture with name " << textureName << "not found\n";
						continue;
					}

					Engine::Core::TextureData* td = nullptr;
					am.getTexture(td, textureName);

					if (td == nullptr)
					{
						std::cerr << "error getting texture with name " << textureName << "\n";
						continue;
					}

					mat.get()->mapTextures[i] = td;

					if (mat.get()->mapTextures[i] == nullptr)
					{
						std::cerr << "texture null\n";
					}
				}
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			std::cout << "Import duration: " << duration << "\n";

		}

	};

}