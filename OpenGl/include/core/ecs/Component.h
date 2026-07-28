#pragma once

#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>

#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/MaterialData.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "core/ECS/ECS.h"
#include "core/input/Keys.h"
namespace Engine::Core::ECS
{
	struct ComponentBase
	{
		virtual ~ComponentBase() = default;
	};

	struct CameraComponent : public ComponentBase
	{
		float fieldOfView{glm::radians(60.0f)};
		float nearClipPlane{0.01f};
		float farClipPlane{ 10.0f };
		bool isOrtho{false};
		float distance{ 5.0f };
		glm::vec3 position{0.0f,0.0f,5.0f};
		glm::mat4 projectionMat{1.0f};
		glm::mat4 viewMat{1.0f};
	};

	struct OrbitalCameraComponent : public ComponentBase
	{
		float yaw{};
		float pitch{};
		float roll{};
	};

	struct ExternalCameraComponent : public ComponentBase
	{
		Entity entityWithCamera{};
	};

	//change this to add more keybindings
	struct PlayerController : ComponentBase
	{
		KeyCode strafeLeft{ KeyCode::A };
		KeyCode strafeRight{ KeyCode::D };
		KeyCode forward{ KeyCode::W };
		KeyCode backward{ KeyCode::S };
		float speed{ 5.0f };
		float turnSensitivity{ 5.0f };
		glm::vec3 baseForward{ 0.0f,0.0f,-1.0f };
	};

	struct TransformComponent : public ComponentBase
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	};

	struct MeshComponent : public ComponentBase
	{
		MeshComponent() = default;
		MeshComponent(Engine::Core::MeshData* mesh) : meshData(mesh) {}
		Engine::Core::MeshData* meshData{ nullptr };
	};

	struct ShaderComponent : public ComponentBase
	{
		ShaderComponent() = default;
		ShaderComponent(Engine::Core::ShaderData* shader) : shaderData(shader) {}
		Engine::Core::ShaderData* shaderData{ nullptr };
	};

	struct MaterialComponent : public ComponentBase
	{
		MaterialComponent() = default;
		MaterialComponent(ShaderData* _shader, MaterialData* _material) : shader{ _shader }, material{ _material } {}
		~MaterialComponent() = default;

		ShaderData* shader{ nullptr };
		MaterialData* material{ nullptr };
	};

	struct MouseInputSettings : ComponentBase
	{
		glm::vec2 sensitivity{ 0.07f,0.05f };
	};

	struct StaticPointLightComponent : ComponentBase
	{
		glm::vec3 color{};
		float radius{};
	};
}