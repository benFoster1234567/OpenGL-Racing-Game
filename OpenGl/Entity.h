#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include <glm/mat4x4.hpp>

class Entity
{
public:
	glm::mat4 transform = glm::mat4(1.0f);
	std::shared_ptr<Material> material = nullptr;
	std::shared_ptr<Mesh> mesh = nullptr;
	Entity() = default;
	
	Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) 
		: mesh(mesh)
		, material(material)
	{}

};
