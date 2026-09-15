#pragma once

#include "core/ecs/coordinator/System.h"
//#include "../coordinator/ECS.h"
#include "PhysicsCommandsAndEvents.h"

#include <vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace Engine::Core::ECS
{
	class Coordinator;

	class PhysicsSystem : public System
	{
	private:
		PhysicsEngineCommandBuffer commandBuffer{};
		std::vector<PhysicsEvent> eventQueue{};

	public:
		void update(Coordinator& coordinator, float deltaTime);

		void fillInitialCommandBuffer(Coordinator& coordinator);

		PhysicsEngineCommandBuffer& getCommandBuffer() { return commandBuffer; }
		const PhysicsEngineCommandBuffer& getCommandBuffer() const { return commandBuffer; } 

		void pollPhysicsEngine(const std::vector<PhysicsEvent>& eventList);

	};
}