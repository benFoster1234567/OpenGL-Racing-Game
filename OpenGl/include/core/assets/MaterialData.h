#pragma once
#include <glm/mat4x4.hpp>
#include "core/assets/TextureData.h"

struct MaterialInfo
{
	glm::vec4 diffuse;
	glm::vec4 ambient;
	glm::vec4 specular;

};

class MaterialData
{
public:
	MaterialData(std::string fileName);

	~MaterialData() = default;

	std::string getName() const { return name; }

	std::string shaderName;

	TextureData* diffuse;
	TextureData* normal;
	MaterialInfo shaderSettings;

private:
	std::string name;
};