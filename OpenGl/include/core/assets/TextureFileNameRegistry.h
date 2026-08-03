#pragma once
#include <cstddef>
#include <unordered_map>
#include <string>
#include <stdexcept>

namespace Engine::Core
{
	using TextureIdx = std::size_t;

	class TextureFileNameRegistry 
	{
	private:
		const TextureIdx MAX_TEXTURES{ 100 };
		TextureIdx nextId{ 0 };
		std::unordered_map<std::string, TextureIdx> filePathToId{};
		std::unordered_map<TextureIdx, std::string> idToFilePath{};
		friend class AssetManager;
	public:
		
		TextureIdx addNewTexture(std::string filePath)
		{
			TextureIdx currentId{ nextId };
			if (filePathToId.contains(filePath))
			{
				throw std::runtime_error("Failed to load texture. Texture was already imported");
			}

			filePathToId[filePath] = currentId;
			idToFilePath[currentId] = filePath;

			nextId++;
		}

		TextureIdx getTextureId(std::string filePath)
		{
			if (!filePathToId.contains(filePath))
			{
				throw std::runtime_error("Failed to retrieve texture ID. filePath is not registered");
			}

			return filePathToId[filePath];
		}

		std::string getFilePath(TextureIdx id)
		{
			if (!idToFilePath.contains(id))
			{
				throw std::runtime_error("Failed to retrieve texture ID. filePath is not registered");
			}

			return idToFilePath[id];
		}

	};
}
