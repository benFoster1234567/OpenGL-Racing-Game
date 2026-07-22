#include "Systems.h"

void Engine::Core::ECS::RenderDispatcherOrbitalCamera::update(Coordinator& coordinator, float aspect)
{
	for (auto entity : entities)
	{
		const auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);
		const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
		const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);
		const auto& orbitalCam = coordinator.getComponent<OrbitalCameraComponent>(entity);

		glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);

		glm::mat4 viewMat = glm::lookAt(transform.position + cameraComp.position, transform.position, { 0,1,0 });

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

void Engine::Core::ECS::MouseControlSystem::update(Coordinator& coordinator, MouseInputResource& mouse)
{
	for (auto entity : entities)
	{
		auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
		auto& orbitalCam = coordinator.getComponent<OrbitalCameraComponent>(entity);

		orbitalCam.pitch += mouse.mouseDelta.y;
		orbitalCam.yaw += mouse.mouseDelta.x;

		//glm::mat4 projectionMat = glm::perspective(cameraComp.fieldOfView,
		//	aspect, cameraComp.nearClipPlane, cameraComp.farClipPlane);
		float r = 5.0f;
		float camx = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::sin(glm::radians(orbitalCam.yaw));
		float camy = r * glm::sin(glm::radians(orbitalCam.pitch));
		float camz = r * glm::cos(glm::radians(orbitalCam.pitch)) * glm::cos(glm::radians(orbitalCam.yaw));
		
		glm::vec3 camPos{ camx, camy, camz };

		cameraComp.position = camPos;


	}
}
