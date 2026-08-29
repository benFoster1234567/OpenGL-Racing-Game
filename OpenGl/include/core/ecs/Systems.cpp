#include "Systems.h"

void Engine::Core::ECS::RenderDispatcherOrbitalCamera::update(Coordinator& coordinator, float aspect)
{
	for (auto entity : entities)
	{
		auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);
		const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
		const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
		const auto& orbitalCam = coordinator.getComponent<OrbitalCameraComponent>(entity);
		const auto& material = coordinator.getComponent<MaterialDataComponent>(entity);

		if (material.material == nullptr)
		{
			throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
		}

		glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);
		cameraComp.projectionMat = projectionMat;
		glm::mat4 viewMat = glm::lookAt(transform.position + cameraComp.position, transform.position, { 0.0f,1.0f,0.0f });
		cameraComp.viewMat = viewMat;
		glm::mat4 transformMat = glm::translate(glm::mat4{1.0f}, transform.position)
			* glm::mat4_cast(transform.rotation)
			* glm::scale(glm::mat4{ 1.0f }, transform.scale);

		//invokes and sends to renderer
		sendRenderInfo.invoke({ .view = viewMat
			, .projection = projectionMat
			, .modelTransform = transformMat
			, .uvScale = meshData.uvScale
			, .shader = shaderData.shaderId
			, .mesh = meshData.meshId
			, .material = material.material });
	}
}

//TODO: Smoothing!
void Engine::Core::ECS::MouseControlSystem::update(Coordinator& coordinator, MouseInputResource& mouse)
{
	for (auto entity : entities)
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

void Engine::Core::ECS::RenderDispatcherExternalCamera::update(Coordinator& coordinator, float aspect)
{
	for (auto entity : entities)
	{
		const auto& extCameraComp = coordinator.getComponent<ExternalCameraComponent>(entity);
		const auto& cameraComp = coordinator.getComponent<CameraComponent>(extCameraComp.entityWithCamera);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);
		const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
		const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
		const auto& material = coordinator.getComponent<MaterialDataComponent>(entity);

		if (material.material == nullptr)
		{
			throw std::runtime_error("MaterialDataComponent is null for entity " + std::to_string(entity));
		}

		glm::mat4 projectionMat = cameraComp.projectionMat;
		glm::mat4 viewMat = cameraComp.viewMat;

		glm::mat4 transformMat = glm::translate(glm::mat4{ 1.0f }, transform.position)
			* glm::mat4_cast(transform.rotation)
			* glm::scale(glm::mat4{ 1.0f }, transform.scale);

		//invokes and sends to renderer
		sendRenderInfo.invoke({ .view = viewMat
			, .projection = projectionMat
			, .modelTransform = transformMat
			, .uvScale = meshData.uvScale
			, .shader = shaderData.shaderId
			, .mesh = meshData.meshId
			, .material = material.material });
	}
}

void Engine::Core::ECS::KeyControlSystem::update(Coordinator& coordinator, const InputBridge& inputHandler, float deltaTime)
{
	for (auto entity : entities)
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
			float turnFactor = glm::clamp(inputKeys.speed* 0.5f, -1.0f, 1.0f);

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


void Engine::Core::ECS::StaticLightRenderSetupSystem::fill(Coordinator& coordinator, std::vector<StaticPointLightRendererData>& queue)
{
	for (auto entity : entities)
	{
		const auto& light = coordinator.getComponent<StaticPointLightComponent>(entity);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);

		StaticPointLightRendererData rd{};
		rd.color = light.color;
		rd.position = transform.position;
		rd.radius = light.radius;
		rd.intensity = light.intensity;

		queue.push_back(rd);
	}
}

void Engine::Core::ECS::PhysicsSystem::update(Coordinator& coordinator, float deltaTime)
{
	for (Entity entity : entities)
	{
		MotionPropertiesComponent& motionProps = coordinator.getComponent<MotionPropertiesComponent>(entity);
		TransformComponent& transform = coordinator.getComponent<TransformComponent>(entity);

		float t{ deltaTime };

		glm::vec3 deltaVelocity = motionProps.acceleration * t;
		glm::vec3 deltaPos = t * (motionProps.velocity + (0.5f * motionProps.acceleration * t));

		motionProps.velocity += deltaVelocity;
		transform.position += deltaPos;
	}
}
