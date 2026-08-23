#pragma once

#include "Component.h"

namespace Engine::Core::ECS
{
	struct MouseComponent : public ComponentBase
	{
		glm::vec2 position{};
		
		size_t pxPosX{};
		size_t pxPosY{};

	};
}