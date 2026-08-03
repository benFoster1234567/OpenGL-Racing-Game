#pragma once
#include <cstddef>
#include <unordered_map>
#include <string>
#include <stdexcept>

#include "core/Patterns.h"
namespace Engine::Core
{
	using TextureId = std::size_t;

	class TextureFileNameRegistry : public Patterns::Singleton<TextureFileNameRegistry>
	{
	private:
		const TextureId MAX_TEXTURES{ 100 };
		TextureId nextId{ 0 };
		std::unordered_map<std::string, TextureId> filePathToId{};
		std::unordered_map<TextureId, std::string> idToFilePath{};

	public:
		
		TextureId addNewTexture(std::string filePath)
		{
			TextureId currentId{ nextId };
			if (filePathToId.contains(filePath))
			{
				throw std::runtime_error("Failed to load texture. Texture was already imported");
			}

			filePathToId[filePath] = currentId;
			idToFilePath[currentId] = filePath;

			nextId++;
		}

		TextureId getTextureId(std::string filePath)
		{
			if (!filePathToId.contains(filePath))
			{
				throw std::runtime_error("Failed to retrieve texture ID. filePath is not registered");
			}

			return filePathToId[filePath];
		}

		std::string getFilePath(TextureId id)
		{
			if (!idToFilePath.contains(id))
			{
				throw std::runtime_error("Failed to retrieve texture ID. filePath is not registered");
			}

			return idToFilePath[id];
		}

	};
}
