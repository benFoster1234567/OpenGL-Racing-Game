#pragma once

#include "core/assets/MeshData.h"
#include "EntityController.h"
#include "core/assets/TextureData.h"
#include <glm/mat4x4.hpp>
#include "core/assets/MaterialData.h"
#include "core/assets/ShaderData.h"

#include <memory>
#include <string>
#include <glm/ext/matrix_transform.hpp>
namespace Engine::Core
{
	using EntityId = size_t;

	struct Transform
	{
		glm::mat4 pos = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
		glm::mat4 rot{1};
		glm::mat4 scale{1};
	};

	class Entity
	{
	private:
		EntityId id;
		friend class EntityManager;
	public:
		Entity() = default;
		Entity(int id, MeshData* mesh, MaterialData* material) : id(id), mesh(mesh), material(material) {};
		Entity(const Entity& other) = default;


		Entity& operator=(const Entity& other);
		
		~Entity() = default;

		EntityController* controller = nullptr;

		Transform transform{};

		MeshData* mesh;
		MaterialData* material;
		
	};

}