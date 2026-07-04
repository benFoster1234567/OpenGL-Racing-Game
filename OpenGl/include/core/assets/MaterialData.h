#pragma once
#include "core/assets/TextureData.h"
#include <string>
#include "ShaderData.h"
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
		MaterialData() = default;
		MaterialData(std::string fileName);

		~MaterialData() = default;

		std::string getName() const { return name; }

		MaterialInfo materialSettings{};

	private:
		std::string name;
	};
}