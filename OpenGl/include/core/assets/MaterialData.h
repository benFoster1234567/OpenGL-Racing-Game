#pragma once
#include "core/assets/TextureData.h"
#include <string>
#include "ShaderData.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <optional>
#include "TextureFileNameRegistry.h"

namespace Engine::Core 
{

	struct MaterialData
	{
		float ns{0.0f};
		glm::vec3 ka{ 1.0f, 1.0f, 1.0f };
		glm::vec3 kd{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ks{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ke{ 0.0f, 0.0f, 0.0f };
		float ni{ 1.0f };
		float d{ 1.0f };
		int illum{ 2 };

		std::string name;
		std::string mapKaPath;
		std::string mapKdPath;
		std::string mapKsPath;
		std::string mapKePath;
		std::string mapBumpPath;
	};
}