#pragma once

#include <glm/vec3.hpp>
#include "../../input/Keys.h"
#include "../coordinator/ECS.h"

namespace Engine::Core::ECS
{
	struct PlayerController : public ComponentBase
	{
		KeyCode strafeLeft{ KeyCode::A };
		KeyCode strafeRight{ KeyCode::D };
		KeyCode forward{ KeyCode::W };
		KeyCode backward{ KeyCode::S };
		float speed{ 0.0f };
		float turnSensitivity{ 5.0f };
		glm::vec3 baseForward{ 0.0f, 0.0f, -1.0f };
	};
}