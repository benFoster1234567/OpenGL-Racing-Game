#pragma once
#include <glm/mat4x4.hpp>
#include "core/assets/TextureData.h"

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

		std::string shaderName;

		TextureData* diffuse{ nullptr };
		TextureData* normal{ nullptr };
		MaterialInfo materialSettings{};

	private:
		std::string name;
	};
}