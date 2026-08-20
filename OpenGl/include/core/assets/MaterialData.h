#pragma once
#include "core/assets/TextureData.h"
#include <string>
#include "ShaderData.h"
#include "AssetIds.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <optional>
#include <array>
#include <algorithm>

namespace Engine::Core 
{
	struct MaterialData
	{
		static constexpr size_t MAX_MAPTYPES = 7;
		enum class MapType : std::size_t
		{
			None = 0,
			Diffuse = 1,
			Specular = 2,
			Normal = 3,
			Shininess = 4,
			Emission = 5,
			Ambient = 6
		};

		std::string name{ "[material name goes here]" };

		float ns{ 0.0f };
		glm::vec3 ka{ 1.0f, 1.0f, 1.0f };
		glm::vec3 kd{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ks{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ke{ 0.0f, 0.0f, 0.0f };
		float ni{ 1.0f };
		float d{ 1.0f };
		int illum{ 2 };

		std::array <std::string, MAX_MAPTYPES> mapFilePaths{};
		std::array <TextureId, MAX_MAPTYPES> mapTextures{};

		static MapType getMapTypeFromString(std::string mapName)
		{
			static bool initialized = false;
			static std::unordered_map<std::string, MapType> stringMap{};
			if (!initialized)
			{
				stringMap["map_Normal"] = MapType::Normal;
				stringMap["map_Ks"] = MapType::Specular;
				stringMap["map_Kd"] = MapType::Diffuse;
				stringMap["map_Ka"] = MapType::Ambient;
				stringMap["map_Ke"] = MapType::Emission;

				stringMap["Normal"] = MapType::Normal;
				stringMap["Specular"] = MapType::Specular;
				stringMap["Diffuse"] = MapType::Diffuse;
				stringMap["Ambient"] = MapType::Ambient;
				stringMap["Emission"] = MapType::Emission;
			}

			if (!stringMap.contains(mapName))
			{
				return MapType::None;
			}

			return stringMap[mapName];
		}

		bool setMapFilePath(std::string mapName, std::string filePath)
		{
			MapType mapType = getMapTypeFromString(mapName);
			if (mapType == MapType::None) return false;
			mapFilePaths[int(mapType)] = filePath;
			return true;
		}

		glm::vec3 getMatColor(const MaterialData::MapType& type) const
		{
			switch (type)
			{
			case MapType::Ambient:
				return ka;
				break;
			case MapType::Diffuse:
				return kd;
				break;
			case MapType::Specular:
				return ks;
				break;
			case MapType::Emission:
				return ke;
				break;
			case MapType::Normal:
				return { 0.5f, 0.5f, 1.0f };
			case MapType::None:
			default:
				return glm::vec3{ 0.0f };
				break;
			};
			return glm::vec3{ 0.0f };

		}

		glm::vec3 getMatColor(std::string mapName) const
		{
			static auto contains = [](std::string str, const std::string& search) -> bool
				{
					std::transform(str.begin(), str.end(), str.begin(),
						[](unsigned char c) { return std::tolower(c); });
					return str.find(search) != std::string::npos;
				};

			MapType type = getMapTypeFromString(mapName);

			return getMatColor(mapName);

		}
	};
}