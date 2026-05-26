#pragma once
//#include "core/assets/MeshData.h"
#include "core/input/Controller.h"
//#include "core/assets/TextureData.h"
#include <glm/mat4x4.hpp>
//#include "core/assets/MaterialData.h"

namespace Engine::Core
{
	class MeshData;
	class MaterialData;

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
		Entity(MeshData* mesh, MaterialData* material) : mesh(mesh), material(material) {};
		~Entity() = default;

		std::unique_ptr<Controller> controller;

		Transform transform{};

		std::string shaderFileName;

		MeshData* mesh;
		MaterialData* material;
	};

}