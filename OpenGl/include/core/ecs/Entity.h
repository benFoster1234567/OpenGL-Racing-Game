#pragma once

#include "core/assets/MeshData.h"
#include "EntityController.h"
#include "core/assets/TextureData.h"
#include <glm/mat4x4.hpp>
#include "core/assets/MaterialData.h"
#include "core/assets/ShaderData.h"

#include <memory>
#include <string>
namespace Engine::Core
{
	using EntityId = size_t;

	struct Transform
	{
		glm::mat4 pos{};
		glm::mat4 rot{};
		glm::mat4 scale{};
	};

	class Entity
	{
	private:
		EntityId id;
		Entity(EntityId id, MeshData* mesh, MaterialData* material) : id(id), mesh(mesh), material(material) {};
		friend class EntityManager;
	public:
		Entity() = default;
		Entity(const Entity& other) = default;


		Entity& operator=(const Entity& other);
		
		~Entity() = default;

		EntityController* controller;

		Transform transform{};

		MeshData* mesh;
		MaterialData* material;
		ShaderData* shader;
	};

}