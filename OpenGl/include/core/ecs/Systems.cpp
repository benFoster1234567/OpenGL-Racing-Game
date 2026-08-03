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
			, .shader = shaderData.shaderData
			, .mesh = meshData.meshData
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
			, .shader = shaderData.shaderData
			, .mesh = meshData.meshData 
			, .material = material.material });
	}
}

void Engine::Core::ECS::KeyControlSystem::update(Coordinator& coordinator, const InputBridge& inputHandler, float deltaTime)
{
	//TODO: fix broken keyHeld method
	for (auto entity : entities)
	{
		const auto& inputKeys = coordinator.getComponent<PlayerController>(entity);
		auto& transform = coordinator.getComponent<TransformComponent>(entity);
		glm::vec3 rotAxis{ 0.0f,1.0f,0.0f };
		glm::vec3 forwards{ 0,0,0 };
		float rotAngle = 0.0f;
		glm::quat rotQuat;

		if (inputHandler.keyPressed(int(inputKeys.strafeLeft)))
		{
			rotAngle += inputKeys.turnSensitivity * deltaTime;
		}

		if (inputHandler.keyPressed(int(inputKeys.strafeRight)))
		{
			rotAngle -= inputKeys.turnSensitivity * deltaTime;
		}

		if (inputHandler.keyPressed(int(inputKeys.forward)))
		{
			forwards.z = deltaTime*5;
		}

		if (inputHandler.keyPressed(int(inputKeys.backward)))
		{
			forwards.z = -deltaTime*5;
		}

		rotQuat = glm::angleAxis(glm::radians(rotAngle), rotAxis);
		rotQuat *= transform.rotation;
		forwards = rotQuat * forwards;
		transform.position += forwards;
		transform.rotation = rotQuat;
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
