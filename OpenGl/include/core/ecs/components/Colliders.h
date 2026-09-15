#pragma once

#include "../coordinator/ECS.h"
#include <glm/ext/vector_float3.hpp>

namespace Engine::Core::ECS
{
	struct BoxColliderComponent : public ComponentBase
	{
		glm::vec3 halfBounds{};
	};
}