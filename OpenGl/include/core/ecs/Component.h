#pragma once

#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/assets/MeshData.h"
#include "core/assets/ShaderData.h"
#include "core/assets/MaterialData.h"
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
		float fieldOfView{ glm::radians(60.0f) };
		float nearClipPlane{ 0.01f };
		float farClipPlane{ 10.0f };
		bool isOrtho{ false };
		float distance{ 5.0f };
		glm::vec3 position{ 0.0f,0.0f,5.0f };
		glm::mat4 projectionMat{ 1.0f };
		glm::mat4 viewMat{ 1.0f };
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

	struct PlayerController : ComponentBase
	{
		KeyCode strafeLeft{ KeyCode::A };
		KeyCode strafeRight{ KeyCode::D };
		KeyCode forward{ KeyCode::W };
		KeyCode backward{ KeyCode::S };
		float speed{ 0.0f };
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
		MeshComponent(Engine::Core::MeshId id) : meshId(id) {}
		Engine::Core::MeshId meshId{};
	};

	struct ShaderComponent : public ComponentBase
	{
		ShaderComponent() = default;
		ShaderComponent(Engine::Core::ShaderId id) : shaderId(id) {}
		Engine::Core::ShaderId shaderId{};
	};

	struct MaterialComponent : public ComponentBase
	{
		MaterialComponent() = default;
		MaterialComponent(Engine::Core::ShaderId _shader, MaterialData* _material)
			: shader{ _shader }, material{ _material } {
		}
		~MaterialComponent() = default;

		Engine::Core::ShaderId shader{};
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
		float intensity{ 1.0f };
	};

	struct MotionPropertiesComponent : ComponentBase
	{
		glm::vec3 velocity{};
		glm::vec3 acceleration{};
	};

	struct MaterialDataComponent : ComponentBase
	{
		MaterialDataComponent() = default;
		MaterialDataComponent(MaterialData* _material) : material{ _material } {}
		~MaterialDataComponent() = default;
		MaterialData* material{ nullptr };
	};

	struct CarControllerComponent : ComponentBase
	{
		KeyCode forward{ KeyCode::W };
		KeyCode backward{ KeyCode::S };
		KeyCode turnLeft{ KeyCode::A };
		KeyCode turnRight{ KeyCode::D };
		float accelerationRate{ 5.0f };
		float turnRate{ 2.0f };
		float frictionFactor{ 0.9f };
	};
}