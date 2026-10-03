#pragma once

#include "../coordinator/System.h"
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
		PhysicsEngineCommandBuffer m_commandBuffer{};
		std::vector<PhysicsEvent> m_eventQueue{};

	public:
		void update(Coordinator& coordinator, float deltaTime);

		void fillInitialCommandBuffer(Coordinator& coordinator);

		PhysicsEngineCommandBuffer& getCommandBuffer() { return m_commandBuffer; }
		const PhysicsEngineCommandBuffer& getCommandBuffer() const { return m_commandBuffer; }

		void pollPhysicsEngine(const std::vector<PhysicsEvent>& eventList);

	};
}