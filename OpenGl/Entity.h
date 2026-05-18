#pragma once
#include "MeshData.h"
#include "Controller.h"
#include "Texture.h"
#include <glm/mat4x4.hpp>
#include "Material.h"


struct Transform
{
	glm::mat4 pos{};
	glm::mat4 rot{};
	glm::mat4 scale{};
};

class Entity
{
public:
	Entity() = default;

	Entity(MeshData* mesh, Material* material) : mesh(mesh), material(material) {};

	std::unique_ptr<Controller> controller;
	
	Transform transform{};
	
	std::string shaderFileName;

	MeshData* mesh;
	Material* material;
};