#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include "../coordinator/ECS.h"

namespace Engine::Core::ECS
{
	struct CameraComponent : public ComponentBase
	{
		float fieldOfView{ glm::radians(60.0f) };
		float nearClipPlane{ 0.01f };
		float farClipPlane{ 10.0f };
		bool isOrtho{ false };
		float distance{ 5.0f };
		glm::vec3 position{ 0.0f, 0.0f, 5.0f };
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
}