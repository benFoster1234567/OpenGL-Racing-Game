#pragma once
#include "../coordinator/ECS.h"
#include <glm/ext/vector_float3.hpp>

namespace Engine::Core::ECS
{
	struct RigidBodyComponent : public ComponentBase
	{
		float mass{ 0.0f };
		bool isKinematic{ false };
		bool isStatic{ false };
		glm::vec3 momentOfInertia{ 0.0f,0.0f,0.0f };
	};
}