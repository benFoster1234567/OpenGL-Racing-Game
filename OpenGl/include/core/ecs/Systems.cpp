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
			, .mesh = meshData.meshData });
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

		float r = 5.0f;
		float camx = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::sin(glm::radians(orbitalCam.yaw));
		float camy = r * glm::sin(glm::radians(orbitalCam.pitch));
		float camz = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::cos(glm::radians(orbitalCam.yaw));
		
		glm::vec3 camPos{ camx, camy, camz };
		cameraComp.position = camPos;
	}
}

void Engine::Core::ECS::RenderDispatcherExternalCamera::update(Coordinator& coordinator, float aspect, Entity cameraEntity)
{
	for (auto entity : entities)
	{
		const auto& cameraComp = coordinator.getComponent<CameraComponent>(cameraEntity);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);
		const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
		const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);

		glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);

		glm::mat4 viewMat = glm::lookAt(transform.position + cameraComp.position, transform.position, { 0,1,0 });

		glm::mat4 transformMat = glm::translate(glm::mat4{ 1.0f }, transform.position)
			* glm::mat4_cast(transform.rotation)
			* glm::scale(glm::mat4{ 1.0f }, transform.scale);

		//invokes and sends to renderer
		sendRenderInfo.invoke({ .view = viewMat
			, .projection = projectionMat
			, .modelTransform = transformMat
			, .shader = shaderData.shaderData
			, .mesh = meshData.meshData });
	}
}
