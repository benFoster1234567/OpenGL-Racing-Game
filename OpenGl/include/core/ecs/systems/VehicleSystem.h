#pragma once
#include "core/ecs/coordinator/System.h"
#include "../../input/KeyboardInput.h"
#include <algorithm>
namespace Engine::Core::ECS
{
	class Coordinator;

	class VehicleSystem : public System
	{
	public:
		void update(Coordinator& coordinator, const InputBridge& input, float deltaTime)
		{
			float maxSteeringValue = 0.5f;
			float steeringSpeed = 1.5f;

			for (const auto& entity : entities)
			{
				auto& vehicleComponent = coordinator.getComponent<VehicleComponent>(entity);
				
				if (input.keyPressed(int(KeyCode::A)))
				{
					vehicleComponent.steeringValue -= steeringSpeed * deltaTime;
				}
				else if (input.keyPressed(int(KeyCode::D)))
				{
					vehicleComponent.steeringValue += steeringSpeed * deltaTime;
				}


				else 
				{
					vehicleComponent.steeringValue = 0.0f;
				}

				if (input.keyPressed(int(KeyCode::W)))
				{
					vehicleComponent.engineForce = 500;
				}
				else
				{
					vehicleComponent.engineForce = vehicleComponent.engineForce < 10? 0 : vehicleComponent.engineForce/5;
				}

				if (input.keyPressed(int(KeyCode::S)))
				{
					vehicleComponent.breakingForce = 50;
				}
				else
				{
					vehicleComponent.breakingForce = 0;
				}

				vehicleComponent.steeringValue = std::clamp(vehicleComponent.steeringValue, -maxSteeringValue, maxSteeringValue);
			}
		}
	};
}
