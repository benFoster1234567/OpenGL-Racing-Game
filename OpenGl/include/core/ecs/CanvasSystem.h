#pragma once
#include "System.h"
#include "Coordinator.h"
#include "ButtonComponent.h"
namespace Engine::Core::ECS
{
	class ButtonSystem : public System
	{
	public:
		void update(Coordinator& coordinator, glm::vec2 mousePosition)
		{
			for (Entity e : entities)
			{
				auto& button = coordinator.getComponent<ButtonComponent>(e);


			}
			
		}
	};
}
