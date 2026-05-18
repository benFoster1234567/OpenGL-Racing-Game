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
	std::string name;

	std::shared_ptr<Texture> texture;
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Shader> shader;

	MaterialInfo shaderSettings;

private:

	
};