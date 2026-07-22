#include "Systems.h"

void Engine::Core::ECS::RenderDispatcherOrbitalCamera::update(Coordinator& coordinator, float aspect)
{
	for (auto entity : entities)
	{
		const auto& cameraComp = coordinator.getComponent<CameraComponent>(entity);
		const auto& transform = coordinator.getComponent<TransformComponent>(entity);
		const auto& meshData = coordinator.getComponent<MeshComponent>(entity);
		const auto& shaderData = coordinator.getComponent<ShaderComponent>(entity);

		glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 1000.0f);

		//glm::mat4 projectionMat = glm::perspective(cameraComp.fieldOfView,
		//	aspect, cameraComp.nearClipPlane, cameraComp.farClipPlane);

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
