#pragma once
#include "../coordinator/ECS.h"
#include <vector>
#include <glm/ext/vector_float3.hpp>

namespace Engine::Core::ECS
{
	struct WheelInfo
	{
		float radius{};
		float suspensionRestLength{};
		glm::vec3 wheelDir{0,-1,0};
		glm::vec3 wheelAxis{-1,0,0};
		glm::vec3 connectionPoint{0,0,0};
		bool isFrontWheel{};

		float suspensionStiffness{ 20.f };
		float wheelsDampingRelaxation{ 2.3f };
		float wheelsDampingCompression{ 4.4f };
		float frictionSlip{ 1000.f };
		float rollInfluence{ .1f };
	};

	struct VehicleComponent : public ComponentBase
	{
		float mass{};
		glm::vec3 collisionBounds{};
		std::vector<WheelInfo> wheels{};
		float engineForce{ 0 };
		float breakingForce{ 0 };
		float steeringValue{ 0 };

	};

}