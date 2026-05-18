#pragma once
#include "MeshData.h"
#include "Controller.h"
#include "Texture.h"
#include <glm/mat4x4.hpp>
#include <optional>

struct Transform
{
	glm::mat4 pos{};
	glm::mat4 rot{};
	glm::mat4 scale{};
};

class Entity
{
public:
	Entity();
	Entity();

	std::unique_ptr<Controller> controller;
	
	Transform transform{};
	
	std::string shaderFileName;

	Texture* texture;
	Texture* normalMap;
	MeshData* mesh;
	Material material;
};