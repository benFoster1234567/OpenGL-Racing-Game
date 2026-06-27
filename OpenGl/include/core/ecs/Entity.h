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
		Entity(int id, MeshData* mesh, ShaderData* shader) :id(id), mesh(mesh), shader(shader) {};
		Entity(const Entity& other) = default;


		Entity& operator=(const Entity& other);
		
		~Entity() = default;

		EntityController* controller = nullptr;

		Transform transform{};
		glm::mat4 transformMatrix{1};
		
		void rotateRadians(float radians, glm::vec3 axis);
		void rotateDegrees(float degrees, glm::vec3 axis);

		//void moveBy(glm::vec3 units);
		//void moveTo(glm::vec3 loc);

		glm::mat4 getTransformMatrix() const noexcept
		{
			return transform.pos * transform.rot * transform.scale;
		}

		ShaderData* shader;
		MeshData* mesh;
		
	};

}