#include "ControlSystems.h"

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>

#include "../coordinator/Coordinator.h"
#include "../../input/KeyboardInput.h"
#include "../components/CameraComponents.h"
#include "../components/MouseInputSettingsComponent.h"
#include "../components/PlayerControllerComponent.h"
#include "../components/TransformComponent.h"

namespace Engine::Core::ECS
{
	void MouseControlSystem::update(Coordinator& coordinator, MouseInputResource& mouse)
	{
		for (auto entity : m_entities)
		{
			auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
			auto& orbitalCam = coordinator.getComponent<OrbitalCameraComponent>(entity);
			const auto& mouseSettings = coordinator.getComponent<MouseInputSettings>(entity);

			float pitchDeltaRad = glm::radians(mouse.mouseDelta.y);
			float yawDeltaRad = glm::radians(mouse.mouseDelta.x);

			orbitalCam.pitch += pitchDeltaRad;
			orbitalCam.yaw += yawDeltaRad;

			float r = cameraComp.distance;
			float camx = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::sin(glm::radians(orbitalCam.yaw));
			float camy = r * glm::sin(glm::radians(orbitalCam.pitch));
			float camz = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::cos(glm::radians(orbitalCam.yaw));

			glm::vec3 camPos{ camx, camy, camz };
			cameraComp.position = camPos;
		}
	}

	void KeyControlSystem::update(Coordinator& coordinator, const InputBridge& inputHandler, float deltaTime)
	{
		for (auto entity : m_entities)
		{
			auto& inputKeys = coordinator.getComponent<PlayerController>(entity);
			auto& transform = coordinator.getComponent<TransformComponent>(entity);

			glm::vec3 rotAxis{ 0.0f, 1.0f, 0.0f };
			glm::vec3 forwards{ 0.0f, 0.0f, 0.0f };
			float rotAngle = 0.0f;

			float accelerationRate = 5.0f;
			float decelerationRate = 4.0f;
			float speedMax = 10.0f;

			if (inputHandler.keyPressed(int(inputKeys.forward)))
			{
				inputKeys.speed += accelerationRate * deltaTime;
			}
			else if (inputHandler.keyPressed(int(inputKeys.backward)))
			{
				inputKeys.speed -= accelerationRate * deltaTime;
			}
			else
			{
				inputKeys.speed = glm::mix(inputKeys.speed, 0.0f, decelerationRate * deltaTime);
				if (glm::abs(inputKeys.speed) < 0.01f)
				{
					inputKeys.speed = 0.0f;
				}
			}

			inputKeys.speed = glm::clamp(inputKeys.speed, -speedMax, speedMax);

			if (glm::abs(inputKeys.speed) > 0.01f)
			{
				float turnFactor = glm::clamp(inputKeys.speed * 0.5f, -1.0f, 1.0f);

				turnFactor *= glm::abs(turnFactor);

				if (inputHandler.keyPressed(int(inputKeys.strafeLeft)))
				{
					rotAngle += inputKeys.turnSensitivity * deltaTime * turnFactor;
				}
				if (inputHandler.keyPressed(int(inputKeys.strafeRight)))
				{
					rotAngle -= inputKeys.turnSensitivity * deltaTime * turnFactor;
				}

				if (glm::abs(rotAngle) > 0.001f)
				{
					glm::quat deltaRot = glm::angleAxis(glm::radians(rotAngle), rotAxis);
					transform.rotation = deltaRot * transform.rotation;
					transform.rotation = glm::normalize(transform.rotation);
				}

				forwards.z = inputKeys.speed * deltaTime;
				transform.position += transform.rotation * forwards;
			}
		}
	}
}