#pragma once

#include "../coordinator/System.h"

namespace Engine::Core
{
	class InputBridge;
	struct MouseInputResource;
}

namespace Engine::Core::ECS
{
	class Coordinator;

	class KeyControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const InputBridge& inputHandler, float deltaTime);
	};

	class MouseControlSystem : public System
	{
	public:
		void update(Coordinator& coordinator, MouseInputResource& mouse);
	};
}