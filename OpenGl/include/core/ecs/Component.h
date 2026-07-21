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
		float fieldOfView{60.0f};
		float nearClipPlane{0.01f};
		float farClipPlane{ 10.0f };
		bool isOrtho{false};
		glm::vec3 position{0.0f,0.0f,0.0f};
	};

	struct TransformComponent : public ComponentBase
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f }; // Quaternion representation
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

	//change this to add more keybindings
	struct KeyInput
	{
		KeyCode strafeLeft{};
		KeyCode strafeRight{};
		KeyCode forward{};
		KeyCode backward{};
	};

	struct MouseInput
	{
		glm::vec2 mousePos{};
	};

	struct OrbitalCameraComponent
	{
		float yaw{};
		float pitch{};
		float roll{};

	};

}