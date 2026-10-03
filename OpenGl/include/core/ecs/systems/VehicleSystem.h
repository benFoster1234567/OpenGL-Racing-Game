#pragma once
#include "core/ecs/coordinator/System.h"
#include "../../input/KeyboardInput.h"
#include <algorithm>
#include <cmath>
namespace Engine::Core::ECS
{
	class Coordinator;

	class VehicleSystem : public System
	{

	public:
		void updateWheels(Coordinator& coordinator)
		{
			for (const auto& entity : m_entities)
			{
				auto& vehicleComponent = coordinator.getComponent<VehicleComponent>(entity);
				const auto& transform = coordinator.getComponent<TransformComponent>(entity);

				float steeringValue = vehicleComponent.steeringValue;

				for (int i = 0; i < vehicleComponent.wheelEntities.size() && i < vehicleComponent.wheels.size(); i++)
				{
					Entity wheelEntity = vehicleComponent.wheelEntities[i];
					auto wheelInfo = vehicleComponent.wheels[i];

					auto& wheelTransform = coordinator.getComponent<TransformComponent>(wheelEntity);

					wheelTransform.position = transform.position + transform.rotation * wheelInfo.connectionPoint;
					wheelTransform.position.y -= wheelInfo.currentSuspensionLength;
					wheelTransform.rotation = transform.rotation;

					if (vehicleComponent.wheels[i].isFrontWheel)
					{
						glm::vec3 euler{ 0, steeringValue, 0 };
						glm::quat q(euler);

						wheelTransform.rotation *= q;
					}
				}

			}
		}

		void update(Coordinator& coordinator, const InputBridge& input, float deltaTime)
		{
			float maxSteeringValue = 0.5f;
			float steeringSpeed = 1.5f;

			for (const auto& entity : m_entities)
			{
				auto& vehicleComponent = coordinator.getComponent<VehicleComponent>(entity);

				if (input.keyPressed(int(KeyCode::A)))
				{
					vehicleComponent.steeringValue += steeringSpeed * deltaTime;
				}

				else if (input.keyPressed(int(KeyCode::D)))
				{
					vehicleComponent.steeringValue -= steeringSpeed * deltaTime;
				}

				else if (std::abs(vehicleComponent.steeringValue) > 0.01f)
				{
					vehicleComponent.steeringValue = glm::mix(vehicleComponent.steeringValue, 0.f, 5 * deltaTime);
				}

				else
				{
					vehicleComponent.steeringValue = 0;
				}

				if (input.keyPressed(int(KeyCode::W)))
				{
					vehicleComponent.engineForce = vehicleComponent.engineStrength;
				}

				else
				{
					vehicleComponent.engineForce = vehicleComponent.engineForce < 10 ? 0 : vehicleComponent.engineForce / 5;
				}

				if (input.keyPressed(int(KeyCode::S)))
				{
					vehicleComponent.breakingForce = vehicleComponent.brakeStrength;
				}

				else
				{
					vehicleComponent.breakingForce = 0;
				}

				vehicleComponent.steeringValue = std::clamp(vehicleComponent.steeringValue, -maxSteeringValue, maxSteeringValue);

			}

			updateWheels(coordinator);
		}
	};
}
