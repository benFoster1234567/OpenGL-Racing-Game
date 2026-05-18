#pragma once
#include <glm/mat4x4.hpp>
#include "Texture.h"

struct MaterialInfo
{
	glm::vec4 diffuse;
	glm::vec4 ambient;
	glm::vec4 specular;

};

class Material
{
public:
	Material(std::string fileName);

	~Material() = default;

	std::string getName() const { return name; }

	std::string shaderName;

	Texture* diffuse;
	Texture* normal;
	MaterialInfo shaderSettings;

private:
	std::string name;
};