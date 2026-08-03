#pragma once
#include "core/assets/TextureData.h"
#include <string>
#include "ShaderData.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace Engine::Core 
{

	struct MaterialInfo
	{
		glm::vec4 diffuse{};
		glm::vec4 ambient{};
		glm::vec4 specular{};

	};

	class MaterialData
	{
	public:
		float ns{0.0f};
		glm::vec3 ka{ 1.0f, 1.0f, 1.0f };
		glm::vec3 kd{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ks{ 1.0f, 1.0f, 1.0f };
		glm::vec3 ke{ 0.0f, 0.0f, 0.0f };
		float ni{ 1.0f };
		float d{ 1.0f };
		int illum{ 2 };

	private:
		std::string name;
	};
}